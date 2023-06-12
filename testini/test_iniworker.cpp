#include <gtest/gtest.h>
#include "iniworker.h"
//基本手动测试
TEST (INIWorkerTest, BasicFunctionality) {
  std::vector<std::string> filenames = {"./test/a.ini"};
  iniWorker m_iniworker;

  for (const std::string& filename : filenames) {
    m_iniworker.parse (filename);
  }

  // 测试存在的键
  EXPECT_TRUE (m_iniworker.lookup ("./test/a.ini", "flag"));
  EXPECT_EQ (m_iniworker.read ("./test/a.ini", "flag"), "1111");

  EXPECT_TRUE (m_iniworker.lookup ("./test/a.ini", "state"));
  EXPECT_EQ (m_iniworker.read ("./test/a.ini", "state"), "2222");

  EXPECT_TRUE (m_iniworker.lookup ("./test/a.ini", "perc"));
  EXPECT_DOUBLE_EQ (m_iniworker.toDouble (m_iniworker.read ("./test/a.ini", "perc")), 0.5);

  // 测试不存在的键
  EXPECT_FALSE (m_iniworker.lookup ("./test/a.ini", "invalid_key"));
  EXPECT_THROW (m_iniworker.read ("./test/a.ini", "invalid_key"), std::runtime_error);

  // 测试文件不存在
  EXPECT_THROW (m_iniworker.parse ("nonexistent.ini"), std::runtime_error);

}

TEST (INIWorkerTest, utf8noBOM) {
  std::vector<std::string> filenames = {"./test/autf8.ini"};
  iniWorker m_iniworker;

  for (const std::string& filename : filenames) {
    m_iniworker.parse (filename);
  }

  // 测试存在的键
  EXPECT_TRUE (m_iniworker.lookup ("./test/autf8.ini", "flag"));
  EXPECT_EQ (m_iniworker.read ("./test/autf8.ini", "flag"), "1111");

  EXPECT_TRUE (m_iniworker.lookup ("./test/autf8.ini", "state"));
  EXPECT_EQ (m_iniworker.read ("./test/autf8.ini", "state"), "2222");

  EXPECT_TRUE (m_iniworker.lookup ("./test/autf8.ini", "perc"));
  EXPECT_DOUBLE_EQ (m_iniworker.toDouble (m_iniworker.read ("./test/autf8.ini", "perc")), 0.5);

  // 测试不存在的键
  EXPECT_FALSE (m_iniworker.lookup ("./test/autf8.ini", "invalid_key"));
  EXPECT_THROW (m_iniworker.read ("./test/autf8.ini", "invalid_key"), std::runtime_error);

  // 测试文件不存在
  EXPECT_THROW (m_iniworker.parse ("nonexistent.ini"), std::runtime_error);

}

TEST (INIWorkerTest, utf8BOM) {
  std::vector<std::string> filenames = {"./test/autf8BOM.ini"};
  iniWorker m_iniworker;

  for (const std::string& filename : filenames) {
    m_iniworker.parse (filename);
  }

  // 测试存在的键
  EXPECT_TRUE (m_iniworker.lookup ("./test/autf8BOM.ini", "flag"));
  EXPECT_EQ (m_iniworker.read ("./test/autf8BOM.ini", "flag"), "1111");

  EXPECT_TRUE (m_iniworker.lookup ("./test/autf8BOM.ini", "state"));
  EXPECT_EQ (m_iniworker.read ("./test/autf8BOM.ini", "state"), "2222");

  EXPECT_TRUE (m_iniworker.lookup ("./test/autf8BOM.ini", "perc"));
  EXPECT_DOUBLE_EQ (m_iniworker.toDouble (m_iniworker.read ("./test/autf8BOM.ini", "perc")), 0.5);

  // 测试不存在的键
  EXPECT_FALSE (m_iniworker.lookup ("./test/autf8BOM.ini", "invalid_key"));
  EXPECT_THROW (m_iniworker.read ("./test/autf8BOM.ini", "invalid_key"), std::runtime_error);

  // 测试文件不存在
  EXPECT_THROW (m_iniworker.parse ("nonexistent.ini"), std::runtime_error);

}

TEST (INIWorkerTest, utf16le) {
  std::vector<std::string> filenames = {"./test/autf16le.ini"};
  iniWorker m_iniworker;

  for (const std::string& filename : filenames) {
    m_iniworker.parse (filename);
  }

  // 测试存在的键
  EXPECT_TRUE (m_iniworker.lookup ("./test/autf16le.ini", "flag"));
  EXPECT_EQ (m_iniworker.read ("./test/autf16le.ini", "flag"), "1111");

  EXPECT_TRUE (m_iniworker.lookup ("./test/autf16le.ini", "state"));
  EXPECT_EQ (m_iniworker.read ("./test/autf16le.ini", "state"), "2222");

  EXPECT_TRUE (m_iniworker.lookup ("./test/autf16le.ini", "perc"));
  EXPECT_DOUBLE_EQ (m_iniworker.toDouble (m_iniworker.read ("./test/autf16le.ini", "perc")), 0.5);

  // 测试不存在的键
  EXPECT_FALSE (m_iniworker.lookup ("./test/autf16le.ini", "invalid_key"));
  EXPECT_THROW (m_iniworker.read ("./test/autf16le.ini", "invalid_key"), std::runtime_error);

  // 测试文件不存在
  EXPECT_THROW (m_iniworker.parse ("nonexistent.ini"), std::runtime_error);

}

TEST (INIWorkerTest, utf16be) {
  std::vector<std::string> filenames = {"./test/autf16be.ini"};
  iniWorker m_iniworker;

  for (const std::string& filename : filenames) {
    m_iniworker.parse (filename);
  }

  // 测试存在的键
  EXPECT_TRUE (m_iniworker.lookup ("./test/autf16be.ini", "flag"));
  EXPECT_EQ (m_iniworker.read ("./test/autf16be.ini", "flag"), "1111");

  EXPECT_TRUE (m_iniworker.lookup ("./test/autf16be.ini", "state"));
  EXPECT_EQ (m_iniworker.read ("./test/autf16be.ini", "state"), "2222");

  EXPECT_TRUE (m_iniworker.lookup ("./test/autf16be.ini", "perc"));
  EXPECT_DOUBLE_EQ (m_iniworker.toDouble (m_iniworker.read ("./test/autf16be.ini", "perc")), 0.5);

  // 测试不存在的键
  EXPECT_FALSE (m_iniworker.lookup ("./test/autf16be.ini", "invalid_key"));
  EXPECT_THROW (m_iniworker.read ("./test/autf16be.ini", "invalid_key"), std::runtime_error);

  // 测试文件不存在
  EXPECT_THROW (m_iniworker.parse ("nonexistent.ini"), std::runtime_error);

}
