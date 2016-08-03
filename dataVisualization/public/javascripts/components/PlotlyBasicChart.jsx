var PlotlyBasicChart = React.createClass({
	
  componentDidMount: function () {
	draw();  
  },
  
  draw: function() {
    Plotly.plot(document.getElementById(this.props.id), this.props.data, this.props.layout);
  },
  
  render: function() {
    return (
      <div id={this.props.id}></div>
    );
  }
});

module.exports = PlotlyBasicChart;
