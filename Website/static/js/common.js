// Common Javascript file for common functions required in the website mainly the graph creator


function roundNumber(number, digits) {
	var multiple = Math.pow(10, digits);
	var rndedNum = Math.round(number * multiple) / multiple;
	return rndedNum;
}

//Returns specified number of unique colors
function makeColorGradient(len) {

	var frequency = 1.665;
	var center = 128;
	var width = 100;
	var colors = [];

	if (len == undefined) {len = 50};
	
    for (var i = 0; i < len; ++i) {
       var red = Math.sin(frequency*i) * width + center;
       var grn = Math.sin(frequency*i + 2) * width + center;
       var blu = Math.sin(frequency*i + 4) * width + center;
       colors[i] = RGB2Color(red,grn,blu);
    }
	
	return colors;
	
}

function RGB2Color(r,g,b) {
    return '#' + byte2Hex(r) + byte2Hex(g) + byte2Hex(b);
}

function byte2Hex(n) {
    var nybHexString = "0123456789ABCDEF";
    return String(nybHexString.substr((n >> 4) & 0x0F,1)) + nybHexString.substr(n & 0x0F,1);
}

function dist(a,b) {
    return Math.sqrt(Math.pow((a.x-b.x),2) + Math.pow((a.y-b.y),2));
}

function errorMessage(itemList, error, itemName, pluralItemName) {
    if (pluralItemName === undefined) {
        pluralItemName = itemName + "s";
    }
    if (itemList.length == 1) {
        return itemName + " " + itemList[0] + " " + error + ".";
    } else if (itemList.length > 1 && itemList.length < 4) {
        return pluralItemName + " " + itemList.slice(0,itemList.length-1).join(", ") 
                    + " and " + itemList[itemList.length-1] + " " + error + ".";
    } else if (itemList.length >= 4) {
        return pluralItemName + " " + itemList.slice(0,4).join(", ") 
                    + " and others " + error + ".";
    }
}

function constraintProportions(o, width, height) {
    var spanX = o['x_max'] - o['x_min']
    var spanY = o['y_max'] - o['y_min']
    if (height != 0 && spanY != 0) {
        var ratio = width / height
        if (spanX / spanY > ratio) {
            var yMiddle = (o['y_min'] + o['y_max']) / 2
            o['y_min'] = yMiddle - spanX / ratio / 2
            o['y_max'] = yMiddle + spanX / ratio / 2
        } else {
            var xMiddle = (o['x_min'] + o['x_max']) / 2
            o['x_min'] = xMiddle - spanY * ratio / 2
            o['x_max'] = xMiddle + spanY * ratio / 2
        }
    }
}
