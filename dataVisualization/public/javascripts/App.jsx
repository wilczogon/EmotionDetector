class PlotlyBasicChart extends React.Component {
	
  componentDidMount() {
	this.draw();  
  }
  
  draw() {
    Plotly.newPlot(document.getElementById(this.props.id), this.props.data, this.props.layout);
  }
  
  render() {
    return (
      <div id={this.props.id}></div>
    );
  }
}

class Charts extends React.Component {
	
  prepare3DPlot(plotData) {
	plotData.push({
		type: 'markers',
		data: {
			x: [0, 0, 500],
			y: [0, 500, 0],
			z: [500, 0, 0]
		}
	});
    return {
		data: plotData.map(function (plotDataPiece) {
			switch(plotDataPiece.type) {
			case 'markers':
				return {
					mode: 'markers',
					marker: {
						color: 'rgb(127, 127, 127)',
						size: 5,
						symbol: 'circle'
					},
					type: 'scatter3d',
					x: plotDataPiece.data.x,
					y: plotDataPiece.data.y,
					z: plotDataPiece.data.z,
				};
			case 'mesh':
				return {
					opacity:0.8,
					color:'rgb(300,100,200)',
					type: 'mesh3d',
					x: plotDataPiece.data.x,
					y: plotDataPiece.data.y,
					z: plotDataPiece.data.z,
				};
			default:
				break;
			};
		}),
		layout: {
		  autosize: false,
		  autorange: false,
		  aspectmode: 'manual',
		  aspectratio: {
			x: 1,
			y: 1,
			z: 1
		  },
		  margin: {
		    l: 0,
			r: 0,
			b: 0,
			t: 0
		}}
	};
  }
  
  prepareScatterPlotMatrixFull(plotData, labelMap, colorMap) {
	var data = [];
	var layout = {};
	var space = 0.04;
	
	var dataMap = {};
	
	plotData.data.expression.forEach(function (expressionNo, index) {
		if(!dataMap[labelMap[expressionNo]]) {
			dataMap[labelMap[expressionNo]] = {};
			
			Object.keys(plotData.data).forEach(function (key) {
				dataMap[labelMap[expressionNo]][key] = [plotData.data[key][index]];
			});
		} else {
			Object.keys(plotData.data).forEach(function (key) {
				dataMap[labelMap[expressionNo]][key].push(plotData.data[key][index]);
			});
		}
	});
	
	console.log(dataMap);
	
	for(var i = 0; i<Object.keys(plotData.data).length-1; ++i)
		for(var j = 0; j<Object.keys(plotData.data).length-1; ++j) {
			var index = j*Object.keys(plotData.data).length + i;
			
			Object.keys(dataMap).forEach(function (expression, no) {
				data.push({
					x: dataMap[expression][Object.keys(plotData.data)[i]],
					y: dataMap[expression][Object.keys(plotData.data)[j]],
					xaxis: 'x' + (index > 0 ? index+1 : ''),
					yaxis: 'y' + (index > 0 ? index+1 : ''),
					type: 'scatter',
					mode: 'markers',
					marker: {
						color: colorMap[expression]
					},
					name: expression,
					showlegend: i == j && i == 0 ? true : false
				});
			});
			layout['xaxis' + (index > 0 ? index+1 : '')] = {domain: [i/(Object.keys(plotData.data).length-1) + space, (i+1)/(Object.keys(plotData.data).length-1) - space], anchor: 'y' + (index > 0 ? index+1 : '')};
			layout['yaxis' + (index > 0 ? index+1 : '')] = {domain: [j/(Object.keys(plotData.data).length-1) + space, (j+1)/(Object.keys(plotData.data).length-1) - space], anchor: 'x' + (index > 0 ? index+1 : '')};
		}
			
    return {
		data: data,
		layout: layout
	}
  }
	
  prepareScatterPlotMatrix(plotData) {
	var data = [];
	var layout = {};
	var space = 0.02;
	
	for(var i = 0; i<Object.keys(plotData.data).length; ++i)
		for(var j = 0; j<plotData.resCols.length; ++j) {
			if(plotData.resCols[j] != Object.keys(plotData.data)[i]) {
				var index = j*Object.keys(plotData.data).length + i;
				data.push({
					x: plotData.data[Object.keys(plotData.data)[i]],
					y: plotData.data[plotData.resCols[j]],
					xaxis: 'x' + (index > 0 ? index+1 : ''),
					yaxis: 'y' + (index > 0 ? index+1 : ''),
					type: 'scatter',
					mode: 'markers',
					name: Object.keys(plotData.data)[i] + ' - ' + plotData.resCols[j]
				});
				layout['xaxis' + (index > 0 ? index+1 : '')] = {domain: [i/(Object.keys(plotData.data).length-1) + space, (i+1)/(Object.keys(plotData.data).length-1) - space], anchor: 'y' + (index > 0 ? index+1 : '')};
				layout['yaxis' + (index > 0 ? index+1 : '')] = {domain: [0, 1], anchor: 'x' + (index > 0 ? index+1 : '')};
			}
		}
			
    return {
		data: data,
		layout: layout
	}
  }
  
  
  prepareScatterPlot(plotData, labelMap, colorMap) {
	var data;
	var layout = {};
	
	data = Object.keys(labelMap).map(function (expressionNo) {
		var indexes = [];
		plotData.data['expression'].forEach(function (expression, index) {
			if(expression == expressionNo)
				indexes.push(index);
		});
		return {
			x: indexes.map(function (index) {return plotData.data['x'][index];}),
			y: indexes.map(function (index) {return plotData.data['y'][index];}),
			type: 'scatter',
			mode: 'markers',
			color: colorMap[expressionNo],
			name: labelMap[expressionNo]
		};
	});
	
    return {
		data: data,
		layout: layout
	}
  }
  
  prepareAnnotatedHeatmap(plotData, labelMap) {
	var data = [];
	var layout = {
		annotations: [],
		xaxis: {
			title: 'Recognized expression'
		},
		yaxis: {
			title: 'Actual expression'
		}
	};
	
	var heatMapData = {};
	
	plotData.data['actual emotion'].forEach(function (actualEmotion, index) {
		var recognizedEmotion = plotData.data['recognized emotion'][index];
		
		if(heatMapData[actualEmotion]) {
			if(heatMapData[actualEmotion][recognizedEmotion]) {
				heatMapData[actualEmotion][recognizedEmotion]++;
			} else {
				heatMapData[actualEmotion][recognizedEmotion] = 1;
			}
		} else {
			heatMapData[actualEmotion] = {};
			heatMapData[actualEmotion][recognizedEmotion] = 1
		}
	});
	
	var keys = Object.keys(labelMap);
	var keysData = keys.map(function (actualEmotion) {
		return keys.map(function (recognizedEmotion) {
			layout.annotations.push({
				xref: 'x1',
				yref: 'y1',
				x: labelMap[recognizedEmotion],
				y: labelMap[actualEmotion],
				text: heatMapData[actualEmotion][recognizedEmotion] ? heatMapData[actualEmotion][recognizedEmotion] : 0,
				font: {
					family: 'Arial',
					size: 12,
					color: 'rgb(50, 171, 96)'
				},
				showarrow: false,
				font: {
					color: 'white'
				}
			});
			return heatMapData[actualEmotion][recognizedEmotion];
		});
	});
	
	data = [{
		x: keys.map(function (key) {
			return labelMap[key];
		}),
		y: keys.map(function (key) {
			return labelMap[key];
		}),
		z: keysData,
		type: 'heatmap',
		//colorscale: colorscaleValue,
		showscale: false
	}];
			
    return {
		data: data,
		layout: layout
	}
  }
  
  render () {
	console.log(this.props.data);
	
	if (this.props.data.Knn)
		var knnData = this.prepareScatterPlotMatrix({data: this.props.data.Knn.data, resCols: ['result']});
	
	if (this.props.data.AdaBoost)
		var adaBoostData = this.prepareScatterPlotMatrix({data: this.props.data.AdaBoost.data, resCols: ['result']});
	
	if (this.props.data.KnnClusters_Knn)
		var knnClustersData = this.prepareScatterPlotMatrixFull({data: this.props.data.KnnClusters_Knn.data},
			{
				0: 'neutral',
				1: 'happy',
				2: 'sad',
				3: 'angry',
				4: 'surprised'
			},{
				'neutral': 'black',
				'happy': 'green',
				'sad': 'blue',
				'angry': 'red',
				'surprised': 'violet'
			}
		);
		
	if (this.props.data.TestResult_Knn)
		var knnHeatmapData = this.prepareAnnotatedHeatmap(
			{
				data: this.props.data.TestResult_Knn.data
			}, {
				1: 'happy',
				2: 'sad',
				3: 'angry',
				4: 'surprised'
			}
		);
		
	if (this.props.data.TestResult_AdaBoost)
		var adaBoostHeatmapData = this.prepareAnnotatedHeatmap(
			{
				data: this.props.data.TestResult_AdaBoost.data
			}, {
				1: 'happy',
				2: 'sad',
				3: 'angry',
				4: 'surprised'
			}
		);
	
	/*if (this.props.data.FacePoints)
		var facePointsData = this.prepare3DPlot([
			{
				type: 'markers',
				data: this.props.data.FacePoints.data
			},
			{
				type: 'mesh',
				data: this.props.data.FacePoints.data
			}
		]);
	
	if (this.props.data.Image && this.props.data.FacePoints)
		var imageData = this.prepare3DPlot([
			{
				type: 'markers',
				data: this.props.data.FacePoints.data
			},
			{
				type: 'mesh',
				data: this.props.data.Image.data
			}
		]);
		
	if (facePointsData) {
		var facePointsPlot = (
			<PlotlyBasicChart data={facePointsData.data} layout={facePointsData.layout} id="face_points" />
		);
	} else {
		var facePointsPlot = null;
	}
	
	if (imageData) {
		var imagePlot = (
			<PlotlyBasicChart data={imageData.data} layout={imageData.layout} id="image" />
		);
	} else {
		var imagePlot = null;
	}*/
	
	if (knnClustersData) {
		knnClustersData.layout.title = 'Przykładowe klastry używane przez metodę knn';
		var knnClustersPlot = (
			<PlotlyBasicChart data={knnClustersData.data} layout={knnClustersData.layout} id="knnClusters" />
		);
	} else {
		var knnClustersPlot = null;
	}
	
	if (knnData) {
		knnData.layout.title = `Zależność między błędem a parametrami wywołania, min error = ${Math.min(...this.props.data.Knn.data.result)}`;
		var knnPlot = (
			<PlotlyBasicChart data={knnData.data} layout={knnData.layout} id="knn" />
		);
	} else {
		var knnPlot = null;
	}
	
	if (adaBoostData) {
		adaBoostData.layout.title = `Zależność między błędem a parametrami wywołania, min error = ${Math.min(...this.props.data.AdaBoost.data.result)}`;
		var adaBoostPlot = (
			<PlotlyBasicChart data={adaBoostData.data} layout={adaBoostData.layout} id="ada_boost" />
		);
	} else {
		var adaBoostPlot = null;
	}
	
	if (knnHeatmapData) {
		knnHeatmapData.layout.title = `Confusion Matrix dla Knn, min error = ${Math.min(...this.props.data.Knn.data.result)}`;
		var knnHeatmapPlot = (
			<PlotlyBasicChart data={knnHeatmapData.data} layout={knnHeatmapData.layout} id="knn_heatmap" />
		);
	} else {
		var knnHeatmapPlot = null;
	}
	
	if (adaBoostHeatmapData) {
		adaBoostHeatmapData.layout.title = `Confusion Matrix dla AdaBoost, min error = ${Math.min(...this.props.data.AdaBoost.data.result)}`;
		var adaBoostHeatmapPlot = (
			<PlotlyBasicChart data={adaBoostHeatmapData.data} layout={adaBoostHeatmapData.layout} id="ada_boost_heatmap" />
		);
	} else {
		var adaBoostHeatmapPlot = null;
	}
	
    return (
      <div>
		{knnPlot}
		{adaBoostPlot}
		{knnClustersPlot}
		{knnHeatmapPlot}
		{adaBoostHeatmapPlot}
	  </div>
    );
  }
}

class App extends React.Component {
  constructor(props) {
    super(props);
    this.state = {};
  }
  
  onClick () {
	var ctx = this;
    $.get(
      `/data?directory=${document.getElementById('directory').value}`,
      function (data) {
		ctx.setState({
			data: data
		});
      }
    );
  }
  
  render () {
	if (this.state.data) {
	  var charts = (
	    <Charts data={this.state.data} />
	  );
	} else {
		var charts = null;
	}
    if(!queryParam)
      queryParam = '';
    return (
      <div className="form-group">
        <label for="directory">Directory name:</label>
        <input type="text" className="form-control" id="directory" defaultValue={queryParam} />
        <button type="button" onClick={this.onClick.bind(this)} className="btn btn-default">Draw charts</button>
		{charts}
      </div>
    );
  }
}

ReactDOM.render(<App />, document.getElementById('app'));
