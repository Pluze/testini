#ifndef INI_WORKER_H
#define INI_WORKER_H

#include <map>
#include <string>
#include <vector>

class iniWorker {
 public:
  iniWorker();

  bool lookup (const std::string& filename, const std::string& key);
  std::string read (const std::string& filename, const std::string& key);
  int toInt (const std::string& str);
  double toDouble (const std::string& str);
  std::string trim (const std::string& str);
  void parse (const std::vector<std::string>& filenames);
  void parse (const std::string& filename);
 private:
  std::map<std::string, std::map<std::string, std::string>> data;
  std::string convertToUTF8 (const std::string& str, const std::string& encoding);
};

#endif  // INI_WORKER_H
