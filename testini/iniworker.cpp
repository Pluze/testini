
#include "iniworker.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

iniWorker::iniWorker() {}

bool iniWorker::lookup (const std::string& filename, const std::string& key) {
  if (data.find (filename) != data.end() && data[filename].find (key) != data[filename].end()) {
    return true;
  }
  return false;
}

std::string iniWorker::read (const std::string& filename, const std::string& key) {
  if (lookup (filename, key)) {
    return data[filename][key];
  }
  throw std::runtime_error ("Key not found in INI file: " + filename);
}

int iniWorker::toInt (const std::string& str) {
  try {
    return std::stoi (str);
  } catch (const std::exception& e) {
    throw std::runtime_error ("Error converting string to int: " + std::string (e.what()));
  }
}

double iniWorker::toDouble (const std::string& str) {
  try {
    return std::stod (str);
  } catch (const std::exception& e) {
    throw std::runtime_error ("Error converting string to double: " + std::string (e.what()));
  }
}

std::string iniWorker::trim (const std::string& str) {
  size_t start = str.find_first_not_of (" \t\r\n");
  size_t end = str.find_last_not_of (" \t\r\n");
  if (start == std::string::npos || end == std::string::npos) {
    return "";
  }
  return str.substr (start, end - start + 1);
}

std::string iniWorker::convertToUTF8 (const std::string& str, const std::string& encoding) {
  if (encoding == "UTF-8" || encoding == "ASCII") return str;
  if (encoding != "UTF-16LE" && encoding != "UTF-16BE") {
    throw std::runtime_error ("Unsupported encoding: " + encoding);
  }

  std::string result;
  if (encoding == "UTF-16BE") {
    for (size_t i = 1; i < str.length(); i += 2) {
      uint16_t ch = static_cast<uint8_t> (str[i]) | static_cast<uint8_t> (str[i - 1]) << 8;
      if (ch < 0x80) {
        result += static_cast<char> (ch);
      } else if (ch < 0x800) {
        result += static_cast<char> (0xc0 | (ch >> 6));
        result += static_cast<char> (0x80 | (ch & 0x3f));
      } else if (ch < 0xd800 || ch > 0xdfff) {
        result += static_cast<char> (0xe0 | (ch >> 12));
        result += static_cast<char> (0x80 | ((ch >> 6) & 0x3f));
        result += static_cast<char> (0x80 | (ch & 0x3f));
      } else {
        int16_t ch2 = str[i + 2] | str[i] << 8;
        uint16_t codepoint = 0x10000 + (((ch & 0x3ff) << 10) | (ch2 & 0x3ff));
        result += static_cast<char> (0xf0 | (codepoint >> 18));
        result += static_cast<char> (0x80 | ((codepoint >> 12) & 0x3f));
        result += static_cast<char> (0x80 | ((codepoint >> 6) & 0x3f));
        result += static_cast<char> (0x80 | (codepoint & 0x3f));
        i += 4;
      }
    }
  } else { // UTF-16LE
    for (size_t i = 0; i < str.length(); i += 2) {
      uint16_t ch = static_cast<uint8_t> (str[i]) | static_cast<uint8_t> (str[i + 1]) << 8;
      if (ch < 0x80) {
        result += static_cast<char> (ch);
      } else if (ch < 0x800) {
        result += static_cast<char> (0xc0 | (ch >> 6));
        result += static_cast<char> (0x80 | (ch & 0x3f));
      } else if (ch < 0xd800 || ch > 0xdfff) {
        result += static_cast<char> (0xe0 | (ch >> 12));
        result += static_cast<char> (0x80 | ((ch >> 6) & 0x3f));
        result += static_cast<char> (0x80 | (ch & 0x3f));
      } else {
        int16_t ch2 = str[i + 2] | str[i + 3] << 8;
        uint16_t codepoint = 0x10000 + (((ch & 0x3ff) << 10) | (ch2 & 0x3ff));
        result += static_cast<char> (0xf0 | (codepoint >> 18));
        result += static_cast<char> (0x80 | ((codepoint >> 12) & 0x3f));
        result += static_cast<char> (0x80 | ((codepoint >> 6) & 0x3f));
        result += static_cast<char> (0x80 | (codepoint & 0x3f));
        i += 4;
      }
    }
  }
  return result;
}


void iniWorker::parse (const std::string& filename) {
  std::ifstream file (filename, std::ios::binary);
  if (!file) {
    throw std::runtime_error ("Failed to open file: " + filename);
  }

  // Read the first few bytes to determine the encoding
  char bom[3] = {0};
  file.read (bom, sizeof (bom));
  std::string encoding;
  try {
    if (file.gcount() >= 2 && static_cast<unsigned char> (bom[0]) == 0xFF && static_cast<unsigned char> (bom[1]) == 0xFE) {
      encoding = "UTF-16LE";
    } else if (file.gcount() >= 2 && static_cast<unsigned char> (bom[0]) == 0xFE
               && static_cast<unsigned char> (bom[1]) == 0xFF) {
      encoding = "UTF-16BE";
    } else if (file.gcount() >= 3 && static_cast<unsigned char> (bom[0]) == 0xEF
               && static_cast<unsigned char> (bom[1]) == 0xBB &&
               static_cast<unsigned char> (bom[2]) == 0xBF) {
      encoding = "UTF-8";
    } else {
      throw std::runtime_error ("Unsupported encoding in file: " + filename);
    }
    // Reset the file stream and skip the BOM bytes
    file.clear();
    file.seekg (encoding == "UTF-8" ? 3 : 2);
  } catch (std::runtime_error& e) {
    std::cout << "Warning: " << e.what() << ". Assuming ASCII encoding.\n";
    encoding = "ASCII";
    file.clear();
    file.seekg (0);
  }


  // Convert the file content to UTF-8 if needed
  std::ostringstream oss;
  oss << file.rdbuf();
  std::string fileContent = oss.str();
  if (encoding != "UTF-8") {
    fileContent = convertToUTF8 (fileContent, encoding);
  }

  // Parse the file content
  std::istringstream iss (fileContent);
  std::string line;
  std::string currentSection;

  int lineNumber = 0;
  while (std::getline (iss, line)) {
    lineNumber++;

    line = trim (line);
    if (line.empty()) {
      continue;
    }

    if (line[0] == '[' && line[line.length() - 1] == ']') {
      currentSection = line.substr (1, line.length() - 2);
    } else {
      size_t delimiterPos = line.find ('=');
      if (delimiterPos != std::string::npos) {
        std::string key = line.substr (0, delimiterPos);
        std::string value = line.substr (delimiterPos + 1);

        key = trim (key);
        value = trim (value);

        if (key.empty()) {
          std::cout << "Warning: Key is missing at line " << lineNumber << " in file " << filename << std::endl;
        } else if (value.empty()) {
          std::cout << "Warning: Value is missing for key '" << key << "' at line " << lineNumber << " in file " << filename <<
                    std::endl;
        } else {
          data[filename][key] = value;
        }
      } else {
        std::cout << "Warning: Invalid line syntax at line " << lineNumber << " in file " << filename << std::endl;
      }
    }
  }

  file.close();
}

void iniWorker::parse (const std::vector<std::string>& filenames) {
  for (const std::string& filename : filenames) {
    try {
      parse (filename);
    } catch (const std::exception& e) {
      std::cout << "Error parsing file: " << filename << ". Reason: " << e.what() << std::endl;
    }
  }
}
