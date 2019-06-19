window.onload = function() {
    document.getElementById("light").src = "images/pic_bulboff.gif";
    document.getElementById("pump").src = "images/gray_water.png";

    update();

    function update() {
        $.getJSON("relays_data.php", function(data) {
            var light_pic;
            var pump_pic;
            /************************ Light *********************/
            if (data[0].light == "OFF") {
                light_pic = "images/pic_bulboff.gif";
            } 
            else if (data[0].light == "ON"){
                light_pic = "images/pic_bulbon.gif";
            }
            else if (data[0].light == "UNCHANGED") {
                for (var i=1; i<data.length; i++) {
                    if (data[i].light == "UNCHANGED") {continue;}
                    else if (data[i].light == "ON") {
                        light_pic = "images/pic_bulbon.gif";
                        break;
                    }
                    else if (data[i].light == "OFF") {
                        light_pic = "images/pic_bulboff.gif";
                        break;
                    }
                }
            }
            document.getElementById("light").src = light_pic;

            /************************ Pump *********************/
            if (data[0].pump == "OFF") {
                pump_pic = "images/gray_water.png";
            } 
            else if (data[0].pump == "ON"){
                pump_pic = "images/blue_water.png";
            }
            else if (data[0].pump == "UNCHANGED") {
                for (var i=1; i<data.length; i++) {
                    if (data[i].pump == "UNCHANGED") {continue;}
                    else if (data[i].pump == "ON") {
                        pump_pic = "images/blue_water.png";
                        break;
                    }
                    else if (data[i].pump == "OFF") {
                        pump_pic = "images/gray_water.png";
                        break;
                    }
                }
            }
            document.getElementById("pump").src = pump_pic;
            setTimeout(function(){update()}, 50);
        });    
    }
}