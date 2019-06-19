window.onload = function() {
    var moisture_dps = [];
    var brightness_dps = [];
    var temperature_dps = [];
    var humidity_dps = [];
    var no_of_dps = 20;

    var moisture_chart = new CanvasJS.Chart("moisture_chart", {
        zoomEnabled:true,
        panEnabled:true,
        animationEnabled:true,
        title:{
            text: "Moisture",
            fontSize: 25,
            fontStyle: "Bold"    
        },
        axisY:{
            title: "%",
            titleFontStyle: "bold",
            minimum: 0
        },
        data: [{
            type: "spline",
            color: "brown",
            dataPoints: moisture_dps
        }]
    });

    var brightness_chart = new CanvasJS.Chart("brightness_chart", {
        zoomEnabled:true,
        panEnabled:true,
        animationEnabled:true,
        title:{
            text: "Brightness",
            fontSize: 25,
            fontStyle: "Bold"
        },
        axisY:{
            title: "lux",
            titleFontStyle: "bold",
            minimum: 0,
            maximum: 800
        },
        data: [{
            type: "line",
            color: "darkblue",
            dataPoints: brightness_dps
        }]
    });

    var temperature_chart = new CanvasJS.Chart("temperature_chart", {
        zoomEnabled:true,
        panEnabled:true,
        animationEnabled:true,
        title:{
            text: "Temperature",
            fontSize: 25,
            fontStyle: "Bold"
        },
        axisY:{
            title: "C degree",
            titleFontStyle: "bold",
            minimum: 0
        },
        data: [{
            type: "spline",
            color: "green",
            dataPoints: temperature_dps
        }]
    });

    var humidity_chart = new CanvasJS.Chart("humidity_chart", {
        zoomEnabled:true,
        panEnabled:true,
        animationEnabled:true,
        title:{
            text: "Humidity",
            fontSize: 25,
            fontStyle: "Bold"
        },
        axisY:{
            title: "%",
            titleFontStyle: "bold",
            minimum: 0
        },
        data: [{
            type: "spline",
            color: "chocolate",
            dataPoints: humidity_dps
        }]
    });

    moisture_chart.render();
    brightness_chart.render();
    temperature_chart.render();
    humidity_chart.render();
    updateChart();

    function updateChart() {
	    $.getJSON("sensors_data.php", function(data) {
	        for(var i=0; i<no_of_dps; i++) {
                moisture_dps.push({"label":data[no_of_dps-i-1].label, "y":data[no_of_dps-i-1].y});
                if (moisture_dps.length > no_of_dps) {
                    moisture_dps.shift();
                }

                brightness_dps.push({"label":data[no_of_dps-i-1+20].label, "y":data[no_of_dps-i-1+20].y});
                if (brightness_dps.length > no_of_dps) {
                    brightness_dps.shift();
                }

                temperature_dps.push({"label":data[no_of_dps-i-1+40].label, "y":data[no_of_dps-i-1+40].y});
                if (temperature_dps.length > no_of_dps) {
                    temperature_dps.shift();
                }

                humidity_dps.push({"label":data[no_of_dps-i-1+60].label, "y":data[no_of_dps-i-1+60].y});
                if (humidity_dps.length > no_of_dps) {
                    humidity_dps.shift();
                }
            };
	        moisture_chart.render();
            brightness_chart.render();
            temperature_chart.render();
            humidity_chart.render();
	        setTimeout(function(){updateChart()}, 50);
	    })
	};
};