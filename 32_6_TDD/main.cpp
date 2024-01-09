#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <string>

class IDBConnection {
public:
  virtual void open(const std::string &dbname, const std::string &user,
                    const std::string &password, const std::string &host,
                    unsigned short port) = 0;
  virtual void close() = 0;
  virtual void execQuery(const std::string &query) = 0;
  virtual ~IDBConnection() {}
};

class DBConnection final : public IDBConnection {
public:
  ~DBConnection() override;
  virtual void open(const std::string &dbname, const std::string &user,
                    const std::string &password, const std::string &host,
                    unsigned short port) override;
  virtual void close() override;
  virtual void execQuery(const std::string &query) override;

private:
  bool is_open{false};
};

class MockDBConnection final : public IDBConnection {
public:
  MOCK_METHOD(void, open,
              (const std::string &dbname, const std::string &user,
               const std::string &password, const std::string &host,
               unsigned short port),
              (override));
  MOCK_METHOD(void, close, (), (override));
  MOCK_METHOD(void, execQuery, (const std::string &query), (override));
};

class ClassThatUsesDb final {
public:
  ClassThatUsesDb() = default;
  explicit ClassThatUsesDb(IDBConnection *dbconn);
  void useConnection(IDBConnection *dbconn);
  void openConnection(const std::string &dbname, const std::string &user,
                      const std::string &password, const std::string &host,
                      unsigned short port);
  void closeConnection();
  void insertRow(const std::string &row);
  IDBConnection *getDbConn() const;
  ~ClassThatUsesDb(){};

private:
  IDBConnection *_dbconn;
};

class ClassThatUsesDbTest : public testing::Test {
public:
  MockDBConnection *dbconn;
  ClassThatUsesDb *ctud;

  void SetUp() {
    dbconn = new MockDBConnection;
    ctud = new ClassThatUsesDb;
    ctud->useConnection(dbconn);
  }
  void TearDown() {
    delete ctud;
    delete dbconn;
  }
};

ClassThatUsesDb::ClassThatUsesDb(IDBConnection *dbconn) : _dbconn{dbconn} {}

void ClassThatUsesDb::useConnection(IDBConnection *dbconn) { _dbconn = dbconn; }

void ClassThatUsesDb::openConnection(const std::string &dbname,
                                     const std::string &user,
                                     const std::string &password,
                                     const std::string &host,
                                     const unsigned short port) {
  _dbconn->open(dbname, user, password, host, port);
}

void ClassThatUsesDb::closeConnection() { _dbconn->close(); }

void ClassThatUsesDb::insertRow(const std::string &row) {
  _dbconn->execQuery(std::string{"INSERT INTO table VALUES("} + row + ")");
}

IDBConnection *ClassThatUsesDb::getDbConn() const { return _dbconn; }

TEST_F(ClassThatUsesDbTest, UseConnectionTest) {
  ASSERT_NE(ctud->getDbConn(), nullptr);
}

TEST_F(ClassThatUsesDbTest, OpenConnectionTest) {
  ASSERT_NE(ctud->getDbConn(), nullptr);
  EXPECT_CALL(*dbconn, open).Times(1);
  ctud->openConnection("testdb", "user", "password", "127.0.0.1", 5432);
}

TEST_F(ClassThatUsesDbTest, CloseConnectionTest) {
  ASSERT_NE(ctud->getDbConn(), nullptr);
  ctud->openConnection("testdb", "user", "password", "127.0.0.1", 5432);
  EXPECT_CALL(*dbconn, close).Times(1);
  ctud->closeConnection();
}

TEST_F(ClassThatUsesDbTest, ExecQueryTest) {
  ASSERT_NE(ctud->getDbConn(), nullptr);
  ctud->openConnection("testdb", "user", "password", "127.0.0.1", 5432);
  EXPECT_CALL(*dbconn, execQuery).Times(1);
  ctud->insertRow("'value1', 'value2', 'value3', 'value4'");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
