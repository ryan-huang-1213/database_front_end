const fs = require("fs").promises;
const sqlite3 = require("sqlite3").verbose();

const db = new sqlite3.Database(
  "./mydatabase.db",
  sqlite3.OPEN_READWRITE | sqlite3.OPEN_CREATE,
  (err) => {
    if (err) {
      console.error("Error opening database " + err.message);
    } else {
      console.log("Database connected!");
    }
  }
);

function executeSqlFile(filePath) {
  return fs.readFile(filePath, "utf8").then(
    (data) =>
      new Promise((resolve, reject) => {
        db.exec(data, (err) => {
          if (err) {
            console.error(`Error executing SQL from ${filePath}`, err);
            reject(err);
          } else {
            console.log(`Executed SQL from ${filePath} successfully`);
            resolve();
          }
        });
      })
  );
}

// Function to execute a SQL query and print results
function executeQuery(sql) {
  return new Promise((resolve, reject) => {
    db.all(sql, [], (err, rows) => {
      if (err) {
        console.error("Error executing query", err);
        reject(err);
      } else {
        console.log(`Query successful: ${sql}`);
        console.log("Results:", rows);
        resolve(rows);
      }
    });
  });
}

async function runScripts() {
  try {
    // await executeSqlFile("data/DDL.sql"); //Init the database if it doesn't exists
    await executeSqlFile("data/smallRelations.sql");
    // Example query
    let sql_query = `
    SELECT * FROM instructor;
    `;
    await executeQuery(sql_query);
    // await executeQuery("SELECT * FROM advisor"); // Replace 'tablename' with your actual table name
  } catch (err) {
    console.error("Failed to execute scripts", err);
  } finally {
    db.close((err) => {
      if (err) {
        console.error("Error closing database", err);
      } else {
        console.log("Database connection closed.");
      }
    });
  }
}

runScripts();
