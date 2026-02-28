<!DOCTYPE html>
<html>
<head>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
<script type="text/javascript">
const URL_BASE_DEFAULT = 'http://en.wikipedia.org/wiki/'; //default is Wikipedia
const URL_VALUE_SOURCE_DEFAULT = "s"; //default is string name

var urlBase = URL_BASE_DEFAULT;
var urlValueSource = URL_VALUE_SOURCE_DEFAULT;

var svg = null;
var svgdoc = null;
var mapData = null;
var nodeIdToNodeObjMap = {}; //a map for faster lookups
var svgElemIdToSvgElemMap = {}; //a map for faster lookups
<?php if (isset($_REQUEST['mode']) && $_REQUEST['mode'] == 'graph') {  ?>
var datasetData = null;
var selectedDataItemId = null;
var selectedDataItemStats = {};
var colorScaleOrig = ['#d2a3f5','#cea3f5','#caa3f5','#c6a3f5','#c3a3f5','#bfa3f5','#bba3f5','#b7a3f5','#b4a3f5','#b0a3f5','#aca3f5','#a8a3f5','#a4a3f5','#a3a6f5','#a3a9f5','#a3adf5','#a3b1f5','#a3b5f5','#a3b9f5','#a3bcf5','#a3c0f5','#a3c4f5','#a3c8f5','#a3cbf5','#a3cff5','#a3d3f5','#a3d7f5','#a3daf5','#a3def5','#a3e2f5','#a3e6f5','#a3eaf5','#a3edf5','#a3f1f5','#a3f5f5','#a3f5f1','#a3f5ed','#a3f5ea','#a3f5e6','#a3f5e2','#a3f5de','#a3f5da','#a3f5d7','#a3f5d3','#a3f5cf','#a3f5cb','#a3f5c8','#a3f5c4','#a3f5c0','#a3f5bc','#a3f5b9','#a3f5b5','#a3f5b1','#a3f5ad','#a3f5a9','#a3f5a6','#a4f5a3','#a8f5a3','#acf5a3','#b0f5a3','#b4f5a3','#b7f5a3','#bbf5a3','#bff5a3','#c3f5a3','#c6f5a3','#caf5a3','#cef5a3','#d2f5a3','#d5f5a3','#d9f5a3','#ddf5a3','#e1f5a3','#e4f5a3','#e8f5a3','#ecf5a3','#f0f5a3','#f4f5a3','#f5f2a3','#f5efa3','#f5eba3','#f5e7a3','#f5e3a3','#f5dfa3','#f5dca3','#f5d8a3','#f5d4a3','#f5d0a3','#f5cda3','#f5c9a3','#f5c5a3','#f5c1a3','#f5bea3','#f5baa3','#f5b6a3','#f5b2a3','#f5aea3','#f5aba3','#f5a7a3','#f5a3a3'];
var colorScaleViridis = [
  '#440154', '#440558', '#450a5c', '#450f61', '#451465', '#461969', '#461e6d', '#472372', 
  '#472876', '#482d7a', '#48317e', '#483682', '#483b85', '#483f89', '#48448d', '#484890', 
  '#474d93', '#475196', '#465698', '#465a9b', '#455e9e', '#4463a0', '#4467a2', '#436ba4', 
  '#4270a5', '#4174a7', '#4078a8', '#3f7ca9', '#3e80ab', '#3d84ac', '#3c88ad', '#3b8cae', 
  '#3a90af', '#3994b0', '#3898b1', '#379cb1', '#35a0b2', '#34a4b2', '#33a8b3', '#32acb3', 
  '#31afb3', '#30b3b4', '#2fb7b4', '#2ebbb4', '#2dbfb4', '#2cc2b4', '#2bc6b4', '#2bcab3', 
  '#2aceb3', '#2ad2b2', '#2ad6b1', '#2bd9b0', '#2bddaf', '#2ee1ad', '#31e5ab', '#35e8a9', 
  '#39eba7', '#3deea4', '#42f2a1', '#47f59e', '#4cf89b', '#52fb98', '#58fe94', '#5eff91', 
  '#64ff8d', '#6aff89', '#71ff85', '#77ff81', '#7eff7d', '#84ff79', '#8bff75', '#92ff71', 
  '#99ff6d', '#a0ff69', '#a7ff65', '#aeff61', '#b5ff5d', '#bcff59', '#c3ff56', '#caff52', 
  '#d1ff4e', '#d8ff4b', '#dfff47', '#e6ff43', '#edff40', '#f3ff3c', '#faff39', '#ffff35', 
  '#ffff37', '#ffff3a', '#ffff3d', '#ffff41', '#ffff45', '#ffff4a', '#ffff4f', '#ffff55', 
  '#ffff5b', '#ffff62', '#ffff69', '#ffff71'
];
var colorScaleMako = [
  '#000004', '#02020b', '#050512', '#080718', '#0b0a1f', '#0e0d26', '#11102d', '#141334', 
  '#17163a', '#1a1941', '#1d1c48', '#211f4f', '#242255', '#27265c', '#2a2a63', '#2d2d69', 
  '#303170', '#333576', '#35397d', '#383d83', '#3a4189', '#3d458f', '#3f4995', '#414e9a', 
  '#4352a0', '#4457a5', '#455bab', '#4660b0', '#4765b5', '#476aba', '#476fbe', '#4774c3', 
  '#4779c7', '#467ecc', '#4583d0', '#4488d4', '#428dd8', '#4192dc', '#3f97df', '#3d9ce3', 
  '#3ba1e6', '#39a6e9', '#37abec', '#35afef', '#33b4f1', '#32b9f3', '#31bef5', '#2fc3f7', 
  '#2ec8f8', '#2dcdf9', '#2cd2fa', '#2bd7fb', '#2bdcfb', '#2be1fb', '#2ce5fb', '#2deafa', 
  '#2eeffa', '#30f3f9', '#33f8f8', '#36fcf7', '#39fff5', '#3dfff3', '#41fff1', '#46fef0', 
  '#4afded', '#4ffceb', '#54fbe9', '#5afae6', '#5ff9e4', '#65f8e1', '#6af6df', '#70f5dc', 
  '#76f4d9', '#7cf2d6', '#82f1d3', '#88efd0', '#8eedcd', '#94ecca', '#9beac7', '#a1e8c4', 
  '#a8e7c1', '#aee5be', '#b5e3bb', '#bbe1b8', '#c2dfb5', '#c8ddb2', '#cfdbaf', '#d5d9ac', 
  '#dcd7a9', '#e2d5a6', '#e9d2a3', '#efd0a1', '#f5ce9e', '#fbcc9c', '#ffca9b', '#ffc89b', 
  '#ffc79d', '#ffc6a0', '#ffc5a4', '#ffc5a8'
];
var  colorScale = colorScaleMako;
//colorScale = colorScale.reverse();
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

function fmtnum(x) {
    var x = x.toString();
    var intPart = x;
    var fractionalPart = '';
    var decimalPos = x.indexOf('.');
    if (decimalPos != -1) {
        intPart = x.substr(0, decimalPos);
        fractionalPart = '.' + x.substr(decimalPos+1, 3);
    }
    intPart = intPart.replace(/\B(?=(\d{3})+(?!\d))/g, ",");
    return intPart + fractionalPart;
}

var nodeMousoverCallback = function(node) {
    return function() {
        var html = node.s;
        if (node.hasOwnProperty('val')) {
            html += '<br/>' + fmtnum(node.val);
        }
        $('#toolTipContainer').html(html);
        $('#toolTipContainer').show();
        setSvgElemFill(getSvgElemByNode(node), mapData.hoverNodeFill);
    }
}

var nodeMouseoutCallback = function(node) {
    return function() {
        $('#toolTipContainer').hide();
        var fill = mapData.defaultNodeFill;
        if (node.hasOwnProperty('fill')) {
            fill = node.fill;
        }
        setSvgElemFill(getSvgElemByNode(node), fill);
    }
}

var nodeClickCallback = function(node) {
    return function() {
        if (node.hasOwnProperty('url')) {
            window.open(node.url);
        } 
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
    if (selectedDataItemId == null) {
        throw new Error("calculateSelectedDataItemStats: selectedDataItemId is null");
    }
    var total = 0;
    var min, max, minNodeId, maxNodeId;
    var n = 0;
    for (var i=0; i<datasetData.data.length; ++i) {
        var record = datasetData.data[i];
        var val = record[selectedDataItemId];
        if (val != undefined) {
            total += val;
            if (min == undefined || val < min) {
                min = val;
                minNodeId = record[datasetData.nodeIdSource];
            }
            if (max == undefined || val > max) {
                max = val;
                maxNodeId = record[datasetData.nodeIdSource];
            }
            n += 1;
        }
    }
    selectedDataItemStats.n = n;
    selectedDataItemStats.total = total;
    selectedDataItemStats.min = min;
    selectedDataItemStats.max = max;
    selectedDataItemStats.minNodeId = minNodeId;
    selectedDataItemStats.maxNodeId = maxNodeId;
    selectedDataItemStats.span = max - min;
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
    var maxNode = getNodeById(selectedDataItemStats.maxNodeId);
    if (!minNode) {
        console.error('getNodeById returned null for minNodeId ' + minNodeId);
    }
    if (!maxNode) {
        console.error('getNodeById returned null for maxNodeId ' + maxNodeId);
    }

    log('Stats:');
    log('n: ' + selectedDataItemStats.n);
    log('total: ' + selectedDataItemStats.total);
    if (minNode && maxNode) {
        log('Min: ' + selectedDataItemStats.min + ' (' + minNode.s + ')');
        log('Max: ' + selectedDataItemStats.max + ' (' + maxNode.s + ')');
    }
    log('Span: ' + selectedDataItemStats.span);
    log('Mean: ' + selectedDataItemStats.mean);
    log('Sigma: ' + selectedDataItemStats.sigma);
    log('MinStdDeviations: ' + selectedDataItemStats.minStdDeviations);
    log('MaxStdDeviations: ' + selectedDataItemStats.maxStdDeviations);
    log('SpanInStdDeviations: ' + selectedDataItemStats.spanInStdDeviations);
    
    $("#statsValue_n").text(fmtnum(selectedDataItemStats.n));
    $("#statsValue_mu").text(fmtnum(selectedDataItemStats.mean));
    $("#statsValue_sigma").text(fmtnum(selectedDataItemStats.sigma));
    if (minNode && maxNode) {
        $("#statsValue_min").text(fmtnum(selectedDataItemStats.min) + ' (' + minNode.s + ')');
        $("#statsValue_max").text(fmtnum(selectedDataItemStats.max) + ' (' + maxNode.s + ')');
    }

    generateScale();
}

var firstPlot = true;

function visualizeSelectedDataItem() {
    if (selectedDataItemId == null) {
        throw new Error("visualizeSelectedDataItem: selectedDataItemId is null");
    } else {
        $('#loaderContainer').show();
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
            if (node != undefined) {
                var urlValue = record[urlValueSource];
                node.url = urlBase + urlValue;
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
            } else {
                console.error(`SVG node is undefined for selectedDataItemId=${selectedDataItemId} record.id=${record.id}`);
            }
        }
        $('#loaderContainer').hide();
    }
}

function generateScale() {
    var html = '';
    for (var i=0; i<colorScale.length; i+=10) {
        html += '<div class="scale-swatch" style="background-color: ' + colorScale[i] + '">';
        if (i==0) {
            html += '-1<a href="http://en.wikipedia.org/wiki/Standard_deviation" class="subtle-link">&sigma;</a>';
            if (selectedDataItemStats.sigma != undefined) {
                var scaleExtremeMin = selectedDataItemStats.mean - selectedDataItemStats.sigma;
                html += '<div class="scale-extrema">(' + fmtnum(scaleExtremeMin) + ')</div>';
            }
        } else if (i==50) {
            html += '<a href="http://en.wikipedia.org/wiki/Mean" class="subtle-link">&mu;</a>';
            if (selectedDataItemStats.sigma != undefined) {
                html += '<div class="scale-extrema">(' + fmtnum(selectedDataItemStats.mean) + ')</div>';
            }
        }
        html += '</div>';
    }
    html += '<div class="scale-swatch" style="background-color: ' + colorScale[colorScale.length-1] + '">';
    html += '1<a href="http://en.wikipedia.org/wiki/Standard_deviation" class="subtle-link">&sigma;</a>';
    var scaleExtremeMax = selectedDataItemStats.mean + selectedDataItemStats.sigma;
    //if (scaleExtremeMax > selectedDataItemStats.max) {
    //    scaleExtremeMax = selectedDataItemStats.max;
    //}
    if (selectedDataItemStats.sigma != undefined) {
        html += '<div class="scale-extrema">(' + fmtnum(selectedDataItemStats.mean + selectedDataItemStats.sigma) + ')</div>';
    }
    html += '</div>';
    $('#scaleContainer').html(html);
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

    const queryString = window.location.search;
    let urlParams = new URLSearchParams(queryString);

    function updateSelectedDataItem(dataItem) {
        selectedDataItemId = dataItem;
        log('selectedDataItemId=' + selectedDataItemId);
        const queryString = window.location.search;
        let urlParams = new URLSearchParams(queryString);
        if (urlParams.get("dataitem") != dataItem) {
            urlParams.set("dataitem", dataItem);
            window.location.search = urlParams; // this causes page to reload, so no need to call visualizeSelectedDataItem
        } else {
            visualizeSelectedDataItem();
        }
    }


    //load data set
    $.getJSON('datasets/<?php echo $_REQUEST['map'] . '/' . $_REQUEST['dataset']; ?>.min.json', function(respData) {
        datasetData = respData;

        if (datasetData.hasOwnProperty("urlBase")) {
            urlBase = datasetData.urlBase;
        }
        if (datasetData.hasOwnProperty("urlValueSource")) {
            urlValueSource = datasetData.urlValueSource;
        }

        let html = '';
        if (datasetData.hasOwnProperty('sourceUrl')) {
            html += '<a href="' + datasetData.sourceUrl + '" class="subtle-link">' + datasetData['name'] + '</a>'
        } else {
            html += '' + datasetData['name'];
        }
        let defaultDataItem = "";
        if ('defaultDataItem' in datasetData) {
            defaultDataItem = datasetData['defaultDataItem'];
        }
        $('#datasetSource').html(html);
        html = 'Data Item: <select id="dataItemSel">';
        for (let i=0; i<datasetData.metadata.length; ++i) {
            let node = datasetData.metadata[i];
            let dataItem = node['Data_Item'];
            let itemDesc = node['Item_Description'];
            let selected = "";
            if ((urlParams.has('dataitem') && urlParams.get('dataitem') == dataItem) 
             || (!urlParams.has('dataitem') && defaultDataItem.length > 0 && defaultDataItem == dataItem)) {
                selected = 'selected="selected" '
                updateSelectedDataItem(dataItem);
            }
            html += '<option '+ selected 
                + 'value="' + dataItem + '">' 
                + dataItem + ' (' + itemDesc + ")"
                + '</option>';
        }
        html += '</select><br/>';
        $('#dataItemSelContainer').html(html);
        $('#dataItemSel').change(function(){
            updateSelectedDataItem($('#dataItemSel option:selected').val());
        });
        setTimeout(function(){$('#loaderContainer').hide()}, 100);
    });
    <?php } else { ?>
        alert('Error: No data set specified.');
    <?php } ?>
<?php } else { ?>
    $('#loaderContainer').hide();
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
    var toolTip = $('#toolTipContainer');
    toolTip.hide();
}

function updateToolTipPos(x,y) {
    $('#toolTipContainer').css('left', x);
    $('#toolTipContainer').css('top', y + 50);
}

$(function() {
    $('#loaderContainer').show();
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
<link rel="stylesheet" href="mapgraph.css">
</head>
<body>
<div id="outerContainer">
    <embed id="svgEmbed" src="maps/<?php echo $_REQUEST["map"]; ?>/map.svg" />

    <?php if (isset($_REQUEST['mode']) && $_REQUEST['mode'] == 'graph') {  ?>
    <div id="scaleContainer"></div>
    <div id="statsContainer">
        <table>
            <tr>
                <th><a href="/assets/projects/mapgraph/doc/A_Brief_Survey_of_Statistical_Symbols.pdf" class="subtle-link">n</a></th>
                <th><a href="http://en.wikipedia.org/wiki/Mean" class="subtle-link">&mu;</a></span></th>
                <th><a href="http://en.wikipedia.org/wiki/Standard_deviation" class="subtle-link">&sigma;</a></span></th>
                <th>min</th>
                <th>max</th>
            </tr>
            <tr>
                <td><span id="statsValue_n">0</span></td>
                <td><span id="statsValue_mu">0</span></td>
                <td><span id="statsValue_sigma">0</span></td>
                <td><span id="statsValue_min">0</span></td>
                <td><span id="statsValue_max">0</span></td>
            </tr>
        </table>
    </div> <!-- statsContainer -->
    <?php } ?>

    <div id="mapSelContainer">
    Map: <select name="map" id="mapSel">
    <?php
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
    ?>
    </select>
    </div> <!-- /mapSelContainer -->

    <div id="dataSetSelContainer">
    Data Set: <select name="dataset" id="datasetSel">
    <option>none</option>
    <?php
    $path = 'datasets/' . $_REQUEST['map'];
    $files = array();
    $d = dir($path);
    while (($entry = $d->read()) !== false) {
        if ($entry != '.' && $entry != '..' && is_file($path . '/' . $entry)) {
            array_push($files, $entry);
        }
    }
    $d->close();

    rsort($files);
    foreach ($files as $file) {
        echo "$file <br>";
        $extPos = strpos($file, '.min.json');
        if ($extPos !== false) {
            $basename = substr($file, 0, $extPos);
            if ($basename == $_REQUEST['dataset']) {
                echo '<option selected="selected">' . $basename . '</option>';        
            } else {
                echo '<option>' . $basename . '</option>';
            }
        }
    }

    ?>
    </select>
    </div> <!-- /dataSetSelContainer -->

    <?php if (isset($_REQUEST['mode']) && $_REQUEST['mode'] == 'graph') {  ?>
    <div id="dataItemSelContainer"></div> <!-- /dataItemSelContainer -->
    <?php } ?>

    <div id="toolTipContainer"></div>

    <div id="loaderContainer">
        <div id="loaderImageContainer">
        <?php
            $size = 2;
            $width = 54*$size;
            $height = 55*$size;
            echo "<img src=\"img/ajax-loader.gif\" width=\"$width\" height=\"$height\"/>";
        ?>
        </div> <!-- /loaderImageContainer -->
    </div> <!-- /loaderContainer -->

    <div id="datasetInfoContainer">
        Source: <span id="datasetSource">0</span>
    </div>

</div> <!-- /outerContainer -->
</body>
</html>
