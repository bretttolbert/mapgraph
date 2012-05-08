var nodeList = []; //list of all node objects
var nodeIdToNodeObjMap = {}; //map of node id to node object (for fast lookups)
var nodeStringToNodeObjMap = {}; //map of node string to node object
var svg = null; //svg <embed> element
var svgdoc = null; //svg document
var selectedNodes = [];
var pathNodes = []; //output of a pathfinding algorithm like bfs
var defaultNodeFill = 'white'; //may be overriden by hidden input element
var af; //adjacency file reset name

/*
var colorScale = ['#ff0000','#ff8000','#ffcd00','#ffff00','#cde600','#80e600','#00ff00','#00ff9a','#00ffff','#00cdff','#0080ff','#0000ff'];
*/
//var colorScale = ['#0000ff','#0080ff','#00cdff','#00ffff','#00ff9a','#00ff00','#80e600','#cde600','#ffff00','#ffcd00','#ff8000','#ff0000'];
var colorScale = ['#d2a3f5','#cea3f5','#caa3f5','#c6a3f5','#c3a3f5','#bfa3f5','#bba3f5','#b7a3f5','#b4a3f5','#b0a3f5','#aca3f5','#a8a3f5','#a4a3f5','#a3a6f5','#a3a9f5','#a3adf5','#a3b1f5','#a3b5f5','#a3b9f5','#a3bcf5','#a3c0f5','#a3c4f5','#a3c8f5','#a3cbf5','#a3cff5','#a3d3f5','#a3d7f5','#a3daf5','#a3def5','#a3e2f5','#a3e6f5','#a3eaf5','#a3edf5','#a3f1f5','#a3f5f5','#a3f5f1','#a3f5ed','#a3f5ea','#a3f5e6','#a3f5e2','#a3f5de','#a3f5da','#a3f5d7','#a3f5d3','#a3f5cf','#a3f5cb','#a3f5c8','#a3f5c4','#a3f5c0','#a3f5bc','#a3f5b9','#a3f5b5','#a3f5b1','#a3f5ad','#a3f5a9','#a3f5a6','#a4f5a3','#a8f5a3','#acf5a3','#b0f5a3','#b4f5a3','#b7f5a3','#bbf5a3','#bff5a3','#c3f5a3','#c6f5a3','#caf5a3','#cef5a3','#d2f5a3','#d5f5a3','#d9f5a3','#ddf5a3','#e1f5a3','#e4f5a3','#e8f5a3','#ecf5a3','#f0f5a3','#f4f5a3','#f5f2a3','#f5efa3','#f5eba3','#f5e7a3','#f5e3a3','#f5dfa3','#f5dca3','#f5d8a3','#f5d4a3','#f5d0a3','#f5cda3','#f5c9a3','#f5c5a3','#f5c1a3','#f5bea3','#f5baa3','#f5b6a3','#f5b2a3','#f5aea3','#f5aba3','#f5a7a3','#f5a3a3'];
//var colorScale = ['#a3bef5','#a3daf5','#a3f5f5','#a3f5da','#a3f5be','#a3f5a3','#bef5a3','#daf5a3','#f5f5a3','#f5daa3','#f5bea3','#f5a3a3'];

var USStatesAndCountiesQuickFacts = function(){
    this.dataSet = [];
    this.dataDict = [];
    this.getRowByFips = function(fips){
        for (var i=0; i<this.dataSet.length; ++i) {
            var row = this.dataSet[i];
            if (row['FIPS'] == fips) {
                return row;
            }
        }
        return null;
    }
}
var quickFacts = new USStatesAndCountiesQuickFacts();
var stats = {};

function getNodeBySvgElemId(id) {
    var node = null;
    if (af == "us-states") {
        node = nodeStringToNodeObjMap[id];
    } else if (af == "us-counties") {
        var nodeId = parseInt(id, 10);
        node = nodeIdToNodeObjMap[nodeId];
    }
    return node;
}

function getSvgElemByNode(node) {
    var svgElem = null;
    if (af == "us-states") {
        svgElem = svgdoc.getElementById(node.nodeString);
    } else if (af == "us-counties") {
        svgElem = svgdoc.getElementById(fipsToString(node.nodeId));
    }
    return svgElem;
}

function setSvgElemFill(elem, fill) {
    $(elem).attr('fill', fill);
    $(elem).css('fill', fill);
}

function setDefaultSvgElemFill(elem) {
    $(elem).attr('fill', defaultNodeFill);
    $(elem).css('fill', defaultNodeFill);   
}

$(function(){
    $('#search').val('');
    $('#baseSpan').hide();
    setTimeout(ready, 500);
});

function fipsToString(fips) {
    var str = fips.toString(10);
    while (str.length < 5) {
        str = '0' + str;
    }
    return str;
}

function nodeToString(node) {
    var result = node.nodeString;
    if (af == "us-states") {
        result += ' (' + node.nodeId + ')';
    } else if (af == "us-counties") {
        result += ' (' + fipsToString(node.nodeId) + ')';
    }
    return result;
}

function log(str) {
    $('#log').val($('#log').val() + str + '\n');
    $('#log')[0].scrollTop = $('#log')[0].scrollHeight;
}

function selectNode(node) {
    var unselected = false;
    var selectedNodeFill = $('#fill').val();
    var selectedNodeIdx = selectedNodes.indexOf(node);
    if (selectedNodeIdx != -1) {
        //unselect
        if (selectedNodes.length = 1) {
            selectedNodes.pop();
            unselected = true;
        } else {
            selectedNodes.splice(selectedNodeIdx, 1);
        }
        svgElem = getSvgElemByNode(node);
        setDefaultSvgElemFill(svgElem);
        /*
        for (var i=0; i<node.neighbors.length; ++i) {
            var neighbor = nodeIdToNodeObjMap[node.neighbors[i]];
            setDefaultSvgElemFill(getSvgElemByNode(neighbor));
        }
        */
    } else {
        selectedNodes.push(node);
        setSvgElemFill(getSvgElemByNode(node), selectedNodeFill);
    }
    var selectedQuickFact = $('#quickFactsSel').val();
    var info = '';
    if (unselected) {
        info += 'Unselected: ';
    } else {
        info += 'Selected: ';
    }
    info += nodeToString(node) + ' Neighbors: ';
    for (var j=0; j<node.neighbors.length; ++j) {
        var neighbor = nodeIdToNodeObjMap[node.neighbors[j]];
        info += nodeToString(neighbor);
        if (j != node.neighbors.length-1) {
            info += ', ';
        }
    }
    var quickFactsRow = quickFacts.getRowByFips(fipsToString(node.nodeId));
    info += ' Quick Facts: ' + quickFactsRow[selectedQuickFact];
    log(info);
}

function clearSelectedNodes() {
    for (var i in selectedNodes) {
        var node = selectedNodes[i];
        selectNode(node);
    }
}

function clearPath() {
    for (var i in pathNodes) {
        var node = pathNodes[i];
        setDefaultSvgElemFill(getSvgElemByNode(node));
    }
    pathNodes = [];
}

function bfs(startNode, goalNode, pathFill) {
    pathDisplayed = true;
    resetNodes(false);
    var q = [];
    q.push(startNode);
    while (q.length > 0) {
        var currentNode = q.shift();
        if (currentNode == goalNode) {
            var node = goalNode.parent;
            var path = [];
            while (node != null && node != startNode) {
                path.push(node);
                setSvgElemFill(getSvgElemByNode(node), pathFill);
                node = node.parent;           
            }
            pathNodes = pathNodes.concat(path);
            return path;
        } else {
            //enqueue neighbors
            //console.log('enqueuing neighbors of ' + nodeToString(currentNode));
            for (var i=0; i<currentNode.neighbors.length; ++i) {
                var neighbor = nodeIdToNodeObjMap[currentNode.neighbors[i]];
                if (neighbor.fill == defaultNodeFill) {
                    neighbor.fill = 'gray';
                    //setSvgElemFill(getSvgElemByNode(neighbor), 'gray');
                    neighbor.parent = currentNode;
                    q.push(neighbor);
                }
            }
        }
    }
}

function greedyColoring(maxTries) {
    $('#loaderContainer').show();
    if (maxTries == 0) {
        alert("Greedy coloring failed. Insufficient number of colors.");
        return;
    }
    for (var tryNum=1; tryNum<=maxTries; ++tryNum) {
        clearSelectedNodes();
        clearPath();
        resetNodes(true);
        var shuffledNodes = [];
        for (var i=0; i<nodeList.length; ++i) {
            shuffledNodes.push(nodeList[i]);
        }
        for (var i=0; i<shuffledNodes.length; ++i) {
            var idx1 = Math.floor(Math.random()*nodeList.length);
            var idx2 = Math.floor(Math.random()*nodeList.length);
            var t = shuffledNodes[idx1];
            shuffledNodes[idx1] = shuffledNodes[idx2];
            shuffledNodes[idx2] = t;
        }
        var availableColors = $('#colors').val().split(',');
        for (var i=0; i<availableColors.length; ++i) {
            availableColors[i] = $.trim(availableColors[i]);
        }
        var coloringFailed = false;
        for (var i in shuffledNodes) {
            var node = shuffledNodes[i];
            var neighborColors = [];
            for (var j in node.neighbors) {
                var neighbor = nodeIdToNodeObjMap[node.neighbors[j]];
                if (neighbor.fill != defaultNodeFill) {
                    neighborColors.push(neighbor.fill);
                }
            }
            var remainingColors = [];
            for (var j in availableColors) {
                if (neighborColors.indexOf(availableColors[j]) == -1) {
                    remainingColors.push(availableColors[j]);
                }
            }
            if (remainingColors.length == 0) {
                coloringFailed = true;
                continue;
            } else {
                var color = remainingColors[0];
                node.fill = color;
                setSvgElemFill(getSvgElemByNode(node), color);
            }
        }
        if (!coloringFailed) {
            //coloring succeeded
            log("Greedy coloring succeeded on try " + tryNum);
            $('#loaderContainer').hide();
            return;
        }
    }
    //coloring failed, no more tries
    log("Greedy coloring failed.");
    $('#loaderContainer').hide();
}

function resetNodes(resetSvg) {
    for (var i=0; i<nodeList.length; ++i) {
        var node = nodeList[i];
        node.fill = defaultNodeFill;
        node.parent = null;
        if (resetSvg) {
            setDefaultSvgElemFill(getSvgElemByNode(node));
        }
    }
}

function calculateStats() {
    var selectedQuickFact = $('#quickFactsSel').val();
    var total = 0;
    var count = 0;
    for (var i=0; i<quickFacts.dataSet.length; ++i) {
        var record = quickFacts.dataSet[i];
        //only consider records that match a known fips
        var fips = parseInt(record['FIPS'], 10);
        if (nodeIdToNodeObjMap.hasOwnProperty(fips)) {
            ++count;
            var val = parseFloat(record[selectedQuickFact]);
            total += val;
            if (stats.minVal == undefined || val < stats.minVal) {
                stats.minVal = val;
                stats.minNode = nodeIdToNodeObjMap[fips];
            }
            if (stats.maxVal == undefined || val > stats.maxVal) {
                stats.maxVal = val;
                stats.maxNode = nodeIdToNodeObjMap[fips];
            }
        }
    }
    stats.span = stats.maxVal - stats.minVal;
    stats.mean = total / count;
    stats.n = count;
    //now calculate standard deviation
    // sigma = sqrt(term1 - term2**2)
    // term1 = sum([i**2 for i in S]) / n
    // term2 = sum([i for i in S]) / n
    var term1 = 0;
    var term2 = 0;
    for (var i=0; i<quickFacts.dataSet.length; ++i) {
        var record = quickFacts.dataSet[i];
        //only consider records that match a known fips
        var fips = parseInt(record['FIPS'], 10);
        if (nodeIdToNodeObjMap.hasOwnProperty(fips)) {
            var val = parseFloat(record[selectedQuickFact]);
            term1 += Math.pow(val,2);
            term2 += val;
        }
    }
    term1 /= stats.n;
    term2 /= stats.n;
    stats.sigma = Math.sqrt(term1 - Math.pow(term2,2));

    //now find max and min std deviations
    var minStdDeviations; //value which is the most std deviations lower than mean
    var maxStdDeviations; //value which is the most std deviations higher than mean
    for (var i=0; i<quickFacts.dataSet.length; ++i) {
        var record = quickFacts.dataSet[i];
        //only consider records that match a known fips
        var fips = parseInt(record['FIPS'], 10);
        if (nodeIdToNodeObjMap.hasOwnProperty(fips)) {
            var val = parseFloat(record[selectedQuickFact]);
            var deviationFromMean = val - stats.mean;
            var stdDeviations = deviationFromMean / stats.sigma;
            if (minStdDeviations == undefined || stdDeviations < minStdDeviations) {
                minStdDeviations = stdDeviations;
            }
            if (maxStdDeviations == undefined || stdDeviations > maxStdDeviations) {
                maxStdDeviations = stdDeviations;
            }
        }
    }
    stats.minStdDeviations = minStdDeviations;
    stats.maxStdDeviations = maxStdDeviations;
    stats.spanInStdDeviations = maxStdDeviations - minStdDeviations;

    if (stats.minNode != undefined && stats.maxNode != undefined) {
        log('Stats:');
        log('n: ' + stats.n);
        log('Min: ' + stats.minVal + ' ' + nodeToString(stats.minNode));
        log('Max: ' + stats.maxVal + ' ' + nodeToString(stats.maxNode));
        log('Span: ' + stats.span);
        log('Mean: ' + stats.mean);
        log('Sigma: ' + stats.sigma);
        log('MinStdDeviations: ' + stats.minStdDeviations);
        log('MaxStdDeviations: ' + stats.maxStdDeviations);
        log('SpanInStdDeviations: ' + stats.spanInStdDeviations);
    }
}

function visualizeQuickFact() {
    $('#loaderContainer').show();
    clearSelectedNodes();
    clearPath();
    resetNodes(true);
    var selectedQuickFact = $('#quickFactsSel').val();
    var scale = $('#scaleSel').val();
    var base = parseInt($('#base').val(), 10);
    var minSigma = parseInt($('#minSigma').val(), 10);
    var maxSigma = parseInt($('#maxSigma').val(), 10);
    calculateStats();
    //var highestPct, highestNode, highestVal;
    for (var i=0; i<quickFacts.dataSet.length; ++i) {
        var record = quickFacts.dataSet[i];
        //only consider records that match a known fips
        var fips = parseInt(record['FIPS'], 10);
        if (nodeIdToNodeObjMap.hasOwnProperty(fips)) {
            var node = nodeIdToNodeObjMap[fips];
            var val = parseFloat(record[selectedQuickFact]);
            if (scale == 'stdDeviation') {
                var deviationFromMean = val - stats.mean;
                var stdDeviations = deviationFromMean / stats.sigma;
                //need to go from minSigma to maxSigma
                var stepWidth = (maxSigma - minSigma) / colorScale.length;
                var currentStep = minSigma;
                for (var j=0; j<colorScale.length; ++j) {
                    currentStep += stepWidth;
                    if (stdDeviations < currentStep || j==colorScale.length-1) {
                        setSvgElemFill(getSvgElemByNode(node), colorScale[j]);
                        break;
                    }
                }
            } else {
                var pct = (val - stats.minVal) / stats.span;
                if (highestPct == undefined || pct > highestPct) {
                    highestPct = pct;
                    highestNode = node;
                    highestVal = val;
                }
                for (var j=colorScale.length-1; j>=0; --j) {
                    var scalePct;
                    if (scale == 'linear') {
                        scalePct = j / colorScale.length;
                    } else if (scale == 'logarithmic') {
                        scalePct = Math.pow(base,j) / Math.pow(base,colorScale.length);
                    }
                    if (pct > scalePct || j==0) {
                        setSvgElemFill(getSvgElemByNode(node), colorScale[j]);
                        break;
                    }
                }
            }
        }
    }
    $('#loaderContainer').hide();
}

function ready() {
    af = $('#af').val();
    defaultNodeFill = $('#defaultNodeFill').val();
    if (af == 'us-counties') {
        $('#colors').val('lightseagreen,lightblue,lightsalmon,beige,plum,lightcoral');
        $('#numTries').val('20');
    } else if (af == 'us-states') {
        $('#colors').val('lightseagreen,lightblue,lightsalmon,beige');
        $('#numTries').val('100');
    }
    //$('#svg').attr('src', '../svg/USA_Counties_with_FIPS_and_names.svg');
    svg = document.getElementById('svg');
    svgdoc = svg.getSVGDocument();
    if (!svgdoc) {
        console.log('SVG Document Failed To Load');
        return;
    }

    $.getJSON('graphgame.cgi?action=getNodes&af='+af, function(data) {
        nodeList = data.nodes;
        nodeIdToNodeObjMap = {};
        nodeStringToNodeObjMap = [];
        $.each(nodeList, function(key, node) {
            node.fill = defaultNodeFill;
            node.parent = null;
            nodeIdToNodeObjMap[node.nodeId] = node;
            nodeStringToNodeObjMap[node.nodeString] = node;
            var svgElem = getSvgElemByNode(node);
            if (svgElem != null) {
                $(svgElem).click(function(){
                    clearPath();
                    selectNode(getNodeBySvgElemId(this.id));             
                });
            } else {
                console.log("Failed to get svg element for " + nodeToString(node));
            }
        });
        $('#search').keyup(function(){
            var query = this.value.toLowerCase();
            if (query.length < nodeList.length.toString(10).length-1) {
                return;
            }
            var results = '';
            for (var i in nodeList) {
                var node = nodeList[i];
                var nodeDesc = nodeToString(node);
                if (nodeDesc.toLowerCase().indexOf(query) != -1) {
                    results += '<a href="#" id="' + node.nodeId + '">'
                        + nodeDesc + '</a><br>';
                }
            }
            $('#searchResults').html(results);
            $('#searchResults a').click(function(){
                selectNode(nodeIdToNodeObjMap[this.id]);
            });
            $('#clearSearch').click(function(){
                $('#searchResults').html('');
                $('#search').val('');
            });
        });
        $('#random').click(function(){
            selectNode(nodeList[Math.floor(Math.random() * nodeList.length)]);
        });
        $('#bfs-demo').click(function(){
            if (selectedNodes.length != 2) {
                alert('Error: Two node must be selected. ' 
                    + selectedNodes.length + ' nodes are currently selected.');
            } else {
                var path1 = bfs(selectedNodes[0], selectedNodes[1], 'orange');
                var path2 = bfs(selectedNodes[1], selectedNodes[0], 'yellow');
                var node1Str = nodeToString(selectedNodes[0]);
                var node2Str = nodeToString(selectedNodes[1]);
                var info = '<b>BFS Results</b>:<br>'
                    + node1Str + ' to ' + node2Str 
                    + ' path length (orange): ' + path1.length + '<br>'
                    + node2Str + ' to ' + node1Str 
                    + ' path length (yellow): ' + path2.length + '<br>';
                $('#info').html($('#info').html() + info);
            }
        });
        $('#greedy-coloring').click(function(){
            var numTries = parseInt($('#numTries').val(), 10);
            greedyColoring(numTries);
        });
        $('#reset').click(function(){
            clearSelectedNodes();
            clearPath();
            resetNodes(true);
        });
    })
    .success(function() { console.log("second success"); })
    .error(function(jqXHR, textStatus, errorThrown) {
            console.log("error " + textStatus);
            console.log("incoming Text " + jqXHR.responseText);
        })
    .complete(function() { console.log("complete"); });

    $.getJSON('graphgame.cgi?action=getUSStatesAndCountiesQuickFactsDataDict', function(data) {
        quickFacts.dataDict = data.data;
        for (var i=0; i<quickFacts.dataDict.length; ++i) {
            var fact = quickFacts.dataDict[i];
            var opt = document.createElement('option');
            opt.value = fact['Data_Item'];
            opt.appendChild(document.createTextNode(fact['Item_Description']));
            $('#quickFactsSel')[0].appendChild(opt);
        }
        $('#quickFactsSel').val('POP010210');
        //$('#quickFactsSel').change(updateInfo);
    })
    .success(function() { console.log("second success"); })
    .error(function(jqXHR, textStatus, errorThrown) {
            console.log("error " + textStatus);
            console.log("incoming Text " + jqXHR.responseText);
        })
    .complete(function() { console.log("complete"); });

    $.getJSON('graphgame.cgi?action=getUSStatesAndCountiesQuickFactsDataSet', function(data) {
        quickFacts.dataSet = data.data;
    })
    .success(function() { console.log("second success"); })
    .error(function(jqXHR, textStatus, errorThrown) {
            console.log("error " + textStatus);
            console.log("incoming Text " + jqXHR.responseText);
        })
    .complete(function() { console.log("complete"); });
    
    $('#visualizeQuickFact').click(function(){
        visualizeQuickFact();
    });

    $('#scaleSel').change(function(){
        var scale = $('#scaleSel').val();
        if (scale == 'logarithmic') {
            $('#baseSpan').show();
        } else {
            $('#baseSpan').hide();
        }
        if (scale == 'stdDeviation') {
            $('#stdDeviationParamsSpan').show();
        } else {
            $('#stdDeviationParamsSpan').hide();
        }
    });

    $('#loaderContainer').hide();
}
