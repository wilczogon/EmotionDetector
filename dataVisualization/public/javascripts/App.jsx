class PlotlyBasicChart extends React.Component {
	
  componentDidMount() {
	this.draw();  
  }
  
  draw() {
    Plotly.plot(document.getElementById(this.props.id), this.props.data, this.props.layout);
  }
  
  render() {
    return (
      <div id={this.props.id}></div>
    );
  }
}

class Charts extends React.Component {
  
  render () {
	console.log(this.props.data);
	
	var facePointsData = [
	  {
	  opacity:0.8,
	  color:'rgb(300,100,200)',
	  type: 'mesh3d',
	  x: this.props.data.FacePoints.data.x,
	  y: this.props.data.FacePoints.data.y,
	  z: this.props.data.FacePoints.data.z,
	  },
	  {
	  mode: 'markers',
	  marker: {
		color: 'rgb(127, 127, 127)',
		size: 5,
		symbol: 'circle'
	  },
	  type: 'scatter3d',
	  x: this.props.data.FacePoints.data.x,
	  y: this.props.data.FacePoints.data.y,
	  z: this.props.data.FacePoints.data.z,
	  }
	];
	
	var facePointsLayout = {
	  margin: {
	  l: 0,
	  r: 0,
	  b: 0,
	  t: 0
    }};
	
    return (
      <div>
		<PlotlyBasicChart data={facePointsData} layout={facePointsLayout} id="face_points" />
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
