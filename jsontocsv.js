const { parse } = require('json2csv');
const fs = require('fs');

// Load your JSON data from the file (ensure it's UTF-8 encoded)
const jsonData = JSON.parse(fs.readFileSync('monster_data.json', 'utf8'));

try {
  const csv = parse(jsonData, {
    excelStrings: false,
  });
  // Add UTF-8 BOM for Excel compatibility
  fs.writeFileSync('dataset.csv', '\ufeff' + csv, 'utf8');
  console.log('CSV file has been saved as dataset.csv');
} catch (err) {
  console.error('An error occurred:', err);
}
