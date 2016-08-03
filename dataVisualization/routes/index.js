var express = require('express');
var router = express.Router();
var fs = require('fs');

function csv2json(csvData) {
	var lines = csvData.trim().split(/\s*\n\s*/);
	var header = lines.splice(0, 1)[0].split(/\s*,\s*/);
	
	return lines.map(function (line) {
		var data = line.split(/\s*,\s*/);
		var jsonEl = {};
		
		for (index in header) {
			jsonEl[header[index]] = data[index];
		}
		
		return jsonEl;
	});
}

function json2plotly(jsonData) {
	var header = Object.keys(jsonData[0]);
	var data = {};
	
	header.forEach(function (columnName) {
		data[columnName] = [];
	});
	
	jsonData.forEach(function (dataPiece) {
		header.forEach(function (columnName) {
			data[columnName].push(dataPiece[columnName]);
		});
	});
	
	return {header: header, data: data};
}

/* GET home page. */
router.get('/', function(req, res, next) {
  var dataDirectory = req.query.directory;
  if(!dataDirectory)
    dataDirectory = '';
  res.render('index', { title: 'Data Visualization', directory: dataDirectory});
});

router.get('/data', function(req, res, next) {
  var dataDirectory = req.query.directory;
  var dataPath = process.argv[2];
  var filesToRead = ['ReduceKnn', 'AdaBoost', 'FacePoints'];
  var errors = [];
  
  var result = {};
  
  filesToRead.forEach(function (nameEl) {
    var fileName = `${dataPath}/${dataDirectory}/${nameEl}_${dataDirectory}.txt`;
    console.log(`Preparing for processing file: ${fileName}...`);
    fs.readFile(`${dataPath}/${dataDirectory}/${nameEl}_${dataDirectory}.txt`, 'utf8', function (err,data) {
      if (err) {
        console.log(err);
		errors.push(err);
      }
      result[nameEl] = json2plotly(csv2json(data));
	  console.log(`Finished processing file: ${fileName}`);
    });
  });
  
  var intervalId = setInterval(function () {
	  if(filesToRead.length === Object.keys(result).length + errors.length) {
		  clearInterval(intervalId);
		  res.json(result);
	  }
  }, 1000);
});

module.exports = router;
