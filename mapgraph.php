<!DOCTYPE html>
<html>
<head>
<!--<script src="http://code.jquery.com/jquery-1.9.1.min.js"></script>-->
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
<script type="text/javascript">
var svg = null;
var svgdoc = null;
var mapData = null;
var nodeIdToNodeObjMap = {}; //a map for faster lookups
var svgElemIdToSvgElemMap = {}; //a map for faster lookups
<?php if (isset($_REQUEST['mode']) && $_REQUEST['mode'] == 'graph') {  ?>
var datasetData = null;
var selectedDataItemId = null;
var selectedDataItemStats = {};
var colorScale = ['#d2a3f5','#cea3f5','#caa3f5','#c6a3f5','#c3a3f5','#bfa3f5','#bba3f5','#b7a3f5','#b4a3f5','#b0a3f5','#aca3f5','#a8a3f5','#a4a3f5','#a3a6f5','#a3a9f5','#a3adf5','#a3b1f5','#a3b5f5','#a3b9f5','#a3bcf5','#a3c0f5','#a3c4f5','#a3c8f5','#a3cbf5','#a3cff5','#a3d3f5','#a3d7f5','#a3daf5','#a3def5','#a3e2f5','#a3e6f5','#a3eaf5','#a3edf5','#a3f1f5','#a3f5f5','#a3f5f1','#a3f5ed','#a3f5ea','#a3f5e6','#a3f5e2','#a3f5de','#a3f5da','#a3f5d7','#a3f5d3','#a3f5cf','#a3f5cb','#a3f5c8','#a3f5c4','#a3f5c0','#a3f5bc','#a3f5b9','#a3f5b5','#a3f5b1','#a3f5ad','#a3f5a9','#a3f5a6','#a4f5a3','#a8f5a3','#acf5a3','#b0f5a3','#b4f5a3','#b7f5a3','#bbf5a3','#bff5a3','#c3f5a3','#c6f5a3','#caf5a3','#cef5a3','#d2f5a3','#d5f5a3','#d9f5a3','#ddf5a3','#e1f5a3','#e4f5a3','#e8f5a3','#ecf5a3','#f0f5a3','#f4f5a3','#f5f2a3','#f5efa3','#f5eba3','#f5e7a3','#f5e3a3','#f5dfa3','#f5dca3','#f5d8a3','#f5d4a3','#f5d0a3','#f5cda3','#f5c9a3','#f5c5a3','#f5c1a3','#f5bea3','#f5baa3','#f5b6a3','#f5b2a3','#f5aea3','#f5aba3','#f5a7a3','#f5a3a3'];
<?php } ?>

function log(msg) {
    console.log(msg);
}

function leftPad(val, digits) {
    var str = val.toString(10);
    while (str.length < digits) {
        str = '0' + str;
    }
    return str;
}

function getNodeById(nodeId) {
    return nodeIdToNodeObjMap[nodeId];
}

function getSvgElemIdByNode(node) {
    var svgElemId = "";
    if (mapData.svgIdSource == "id") {
        svgElemId = leftPad(node.id, mapData.nodeMetadata.id.digits);
    } else {
        svgElemId = node[mapData.svgIdSource];
    }
    return svgElemId;
}

function getSvgElemByNode(node) {
    var svgElem = null;
    if (mapData.svgIdSource == "id") {
        var svgElemId = leftPad(node.id, mapData.nodeMetadata.id.digits);
        return svgElemIdToSvgElemMap[svgElemId];
    } else {
        var svgElemId = node[mapData.svgIdSource];
        return svgElemIdToSvgElemMap[svgElemId];
    }
    return svgElem;
}

function setSvgElemFill(svgElem, fill) {
    var elem = $(svgElem);
    elem.css('fill', fill);
    elem.attr('fill', fill);
    elem.children().css('fill', fill);
    elem.children().attr('fill', fill);
}

function numberWithCommas(x) {
    return x.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
}

var nodeMousoverCallback = function(node) {
    return function() {
        var html = node.s;
        if (node.hasOwnProperty('val')) {
            html += '<br/>' + numberWithCommas(node.val);
        }
        $('#toolTip').html(html);
        $('#toolTip').show();
        setSvgElemFill(getSvgElemByNode(node), mapData.hoverNodeFill);
    }
}

var nodeMouseoutCallback = function(node) {
    return function() {
        $('#toolTip').hide();
        var fill = mapData.defaultNodeFill;
        if (node.hasOwnProperty('fill')) {
            fill = node.fill;
        }
        setSvgElemFill(getSvgElemByNode(node), fill);
    }
}

var nodeClickCallback = function(node) {
    return function() {
        var url = 'http://en.wikipedia.org/wiki/' + node.s.replace(' ','_');
        window.open(url);
    }
}

function resetNodes() {
    for (var i=0; i<mapData.nodes.length; ++i) {
        var node = mapData.nodes[i];
        setSvgElemFill(getSvgElemByNode(node), mapData.defaultNodeFill);
        delete node.fill;
        delete node.val;
    }
}

<?php if (isset($_REQUEST['mode']) && $_REQUEST['mode'] == 'graph') {  ?>
function calculateSelectedDataItemStats() {
    var total = 0;
    var minVal, maxVal, minNodeId, maxNodeId;
    var n = 0;
    for (var i=0; i<datasetData.data.length; ++i) {
        var record = datasetData.data[i];
        var val = record[selectedDataItemId];
        if (val != undefined) {
            total += val;
            if (minVal == undefined || val < minVal) {
                minVal = val;
                minNodeId = record[datasetData.nodeIdSource];
            }
            if (maxVal == undefined || val > maxVal) {
                maxVal = val;
                maxNodeId = record[datasetData.nodeIdSource];
            }
            n += 1;
        }
    }
    selectedDataItemStats.n = n;
    selectedDataItemStats.total = total;
    selectedDataItemStats.minVal = minVal;
    selectedDataItemStats.maxVal = maxVal;
    selectedDataItemStats.minNodeId = minNodeId;
    selectedDataItemStats.maxNodeId = maxNodeId;
    selectedDataItemStats.span = maxVal - minVal;
    selectedDataItemStats.mean = total / n;
    
    //now calculate standard deviation
    // sigma = sqrt(term1 - term2**2)
    // term1 = sum([i**2 for i in S]) / n
    // term2 = sum([i for i in S]) / n
    var term1 = 0;
    var term2 = 0;
    for (var i=0; i<datasetData.data.length; ++i) {
        var record = datasetData.data[i];
        var val = record[selectedDataItemId];
        if (val != undefined) {
            term1 += Math.pow(val,2);
            term2 += val;
        }
    }
    term1 /= n;
    term2 /= n;
    selectedDataItemStats.sigma = Math.sqrt(term1 - Math.pow(term2,2));

    //now find max and min std deviations
    var minStdDeviations; //value which is the most std deviations lower than mean
    var maxStdDeviations; //value which is the most std deviations higher than mean
    for (var i=0; i<datasetData.data.length; ++i) {
        var record = datasetData.data[i];
        var val = record[selectedDataItemId];
        if (val != undefined) {
            var deviationFromMean = val - selectedDataItemStats.mean;
            var stdDeviations = deviationFromMean / selectedDataItemStats.sigma;
            if (minStdDeviations == undefined || stdDeviations < minStdDeviations) {
                minStdDeviations = stdDeviations;
            }
            if (maxStdDeviations == undefined || stdDeviations > maxStdDeviations) {
                maxStdDeviations = stdDeviations;
            }
        }
    }
    selectedDataItemStats.minStdDeviations = minStdDeviations;
    selectedDataItemStats.maxStdDeviations = maxStdDeviations;
    selectedDataItemStats.spanInStdDeviations = maxStdDeviations - minStdDeviations;
    
    var minNode = getNodeById(selectedDataItemStats.minNodeId);
    if (!minNode) {
        console.log('Error: getNodeById returned null for minNodeId ' + minNodeId);
    }
    var maxNode = getNodeById(selectedDataItemStats.maxNodeId);
    if (!maxNode) {
        console.log('Error: getNodeById returned null for maxNodeId ' + maxNodeId);
    }
    
    log('Stats:');
    log('n: ' + selectedDataItemStats.n);
    log('total: ' + selectedDataItemStats.total);
    log('Min: ' + selectedDataItemStats.minVal + ' (' + minNode.s + ')');
    log('Max: ' + selectedDataItemStats.maxVal + ' (' + maxNode.s + ')');
    log('Span: ' + selectedDataItemStats.span);
    log('Mean: ' + selectedDataItemStats.mean);
    log('Sigma: ' + selectedDataItemStats.sigma);
    log('MinStdDeviations: ' + selectedDataItemStats.minStdDeviations);
    log('MaxStdDeviations: ' + selectedDataItemStats.maxStdDeviations);
    log('SpanInStdDeviations: ' + selectedDataItemStats.spanInStdDeviations);
}

var firstPlot = true;
function visualizeSelectedDataItem() {
    if (firstPlot) {
        firstPlot = false;
    } else {
        resetNodes();
    }
    //$('#loaderContainer').show();
    //clearSelectedNodes();
    //clearPath();
    //resetNodes(true);
    //var selectedQuickFact = $('#quickFactsSel').val();
    //var scale = $('#scaleSel').val();
    //var base = parseInt($('#base').val(), 10);
    var minSigma = -1; //var minSigma = parseInt($('#minSigma').val(), 10);
    var maxSigma = 1; //var maxSigma = parseInt($('#maxSigma').val(), 10);
    calculateSelectedDataItemStats();
    //var highestPct, highestNode, highestVal;
    for (var i=0; i<datasetData.data.length; ++i) {
        var record = datasetData.data[i];
        var node = getNodeById(record[datasetData.nodeIdSource]);
        var val = record[selectedDataItemId];
        if (val != undefined) {
            node.val = val;
            var deviationFromMean = val - selectedDataItemStats.mean;
            var stdDeviations = deviationFromMean / selectedDataItemStats.sigma;
            //log('node ' + node.s + ' stdDeviations: ' + stdDeviations);
            //need to go from minSigma to maxSigma
            var stepWidth = (maxSigma - minSigma) / colorScale.length;
            var currentStep = minSigma;
            for (var j=0; j<colorScale.length; ++j) {
                currentStep += stepWidth;
                if (stdDeviations < currentStep || j==colorScale.length-1) {
                    node.fill = colorScale[j];
                    setSvgElemFill(getSvgElemByNode(node), node.fill);
                    break;
                }
            }
        }
    }
    //$('#loaderContainer').hide();
}
<?php } ?>

function svgLoadCallback() {
    svg = $('#svgEmbed')[0];
    if (svg.contentDocument) {
        log('svg.contentDocument != null');
        svgdoc = svg.contentDocument;
    } else if (svg.getSVGDocument()) {
        log('svg.getSVGDocument() != null');
        svgdoc = svg.getSVGDocument();
    } else {
        alert('SVG document failed to load');
        return;
    }
    
    //load map definition
    $.getJSON('maps/<?php echo $_REQUEST['map']; ?>/map.min.json', function(respData) {
        mapData = respData;
        for (var i=0; i<mapData.nodes.length; ++i) {
            var node = mapData.nodes[i];
            nodeIdToNodeObjMap[node.id] = node;
            var svgElemId = getSvgElemIdByNode(node);
            svgElemIdToSvgElemMap[svgElemId] = svgdoc.getElementById(svgElemId);
            var elem = $(getSvgElemByNode(node));
            elem.mouseover(nodeMousoverCallback(node));
            elem.mouseout(nodeMouseoutCallback(node));
            elem.mousemove(function(evt){
                updateToolTipPos(evt.pageX, evt.pageY);
            });
            elem.click(nodeClickCallback(node));
        }
    });
    
<?php if (isset($_REQUEST['mode']) && $_REQUEST['mode'] == 'graph') {  ?>
    <?php if (isset($_REQUEST['dataset'])) { ?>
    //load data set
    $.getJSON('datasets/<?php echo $_REQUEST['map'] . '/' . $_REQUEST['dataset']; ?>.min.json', function(respData) {
        datasetData = respData;
        var html = 'Data Item: <select>';
        for (var i=0; i<datasetData.metadata.length; ++i) {
            var node = datasetData.metadata[i];
            s = node['Data_Item'] + ' ' + node['Item_Description']
            html += '<option value="' + node['Data_Item'] + '">' + s + '</option>';
        }
        html += '</select><br/>';
        $('#dataItemSel').html(html);
        $('#dataItemSel').change(function(){
            selectedDataItemId = $('#dataItemSel option:selected').val();
            log('selectedDataItemId=' + selectedDataItemId);
            visualizeSelectedDataItem();
        });
    });
    <?php } else { ?>
        alert('Error: No data set specified.');
    <?php } ?>
<?php } ?>

    $('#mapSel').change(function(){
        var selectedMap = $('#mapSel option:selected').text();
        window.location = 'mapgraph.php?map=' + selectedMap;
    });
    $('#datasetSel').change(function(){
        var selectedMap = $('#mapSel option:selected').text();
        var selectedDataset = $('#datasetSel option:selected').text();
        if (selectedDataset == 'none') {
            window.location = 'mapgraph.php?map=' + selectedMap;
        } else {
            window.location = 'mapgraph.php?map=' + selectedMap + '&mode=graph&dataset=' + selectedDataset;
        }
    });
}

function initToolTip() {
    var toolTip = $('#toolTip');
    toolTip.hide();
    toolTip.css({
        'position':'absolute',
        'left':'0',
        'top':'0',
        'border':'1px solid black',
        'background-color': 'white',
    });
}

function updateToolTipPos(x,y) {
    $('#toolTip').css('left', x);
    $('#toolTip').css('top', y + 50);
}

<?php if (isset($_REQUEST['mode']) && $_REQUEST['mode'] == 'graph') {  ?>
function generateScale() {
    var html = '';
    //html += '<div style="float:left">Scale: </div>';
    for (var i=0; i<colorScale.length; i+=10) {
        html += '<div class="ScaleSwatch" style="background-color: ' + colorScale[i] + '">';
        if (i==0) {
            html += '&le;-1&sigma;';
        } else if (i==50) {
            html += 'x&#772;';
        }
        html += '</div>';
    }
    html += '<div class="ScaleSwatch" style="background-color: ' + colorScale[colorScale.length-1] + '">';
    html += '&ge;1&sigma;';
    html += '</div>';
    $('#scale').html(html);
}
<?php } ?>

$(function() {
    $('#svgEmbed')[0].addEventListener('load', svgLoadCallback, false);
    initToolTip();
    if (!window.chrome) {
        log('Warning: MapGraph works best in Google Chrome.');
    } else {
        log('Google Chrome detected');
    }
<?php if (isset($_REQUEST['mode']) && $_REQUEST['mode'] == 'graph') {  ?>
    generateScale();
<?php } ?>
});
</script>
<style type="text/css">
.ScaleSwatch {
    width: 50px;
    height: 30px;
    float: left;
    text-align: center;
    padding-top: 10px;
}
</style>
</head>
<body>
<div id="toolTip"></div>
<embed id="svgEmbed" src="maps/<?php echo $_REQUEST["map"]; ?>/map.svg" />
<?php if (isset($_REQUEST['mode']) && $_REQUEST['mode'] == 'graph') {  ?>
<div id="scale"></div>
<br/><br/>
<?php } ?>
<br/>
<?php
//map select
echo 'Map: <select name="map" id="mapSel">';
$d = dir("maps");
while (($entry = $d->read()) !== false) {
    if ($entry != '.' && $entry != '..' && is_dir("maps/" . $entry)) {
        if ($entry == $_REQUEST['map']) {
            echo '<option selected="selected">' . $entry . '</option>';        
        } else {
            echo '<option>' . $entry . '</option>';
        }
    }
}
$d->close();
echo '</select><br/>';

//dataset select
echo 'Data Set: <select name="dataset" id="datasetSel">';
echo '<option>none</option>';
$path = 'datasets/' . $_REQUEST['map'];
$d = dir($path);
while (($entry = $d->read()) !== false) {
    if ($entry != '.' && $entry != '..' && is_file($path . '/' . $entry)) {
        $extPos = strpos($entry, '.min.json');
        if ($extPos !== false) {
            $entry = substr($entry, 0, $extPos);
            if ($entry == $_REQUEST['dataset']) {
                echo '<option selected="selected">' . $entry . '</option>';        
            } else {
                echo '<option>' . $entry . '</option>';
            }
        }
    }
}
$d->close();
echo '</select>';
?>
<br/>
<?php if (isset($_REQUEST['mode']) && $_REQUEST['mode'] == 'graph') {  ?>
<div id="dataItemSel"></div>
<?php } ?>
</body>
</html>