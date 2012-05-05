var nodeList = []; //list of all node objects
var nodeIdToNodeObjMap = {}; //map of node id to node object (for fast lookups)
var nodeStringToNodeObjMap = {}; //map of node string to node object
var svg = null; //svg <embed> element
var svgdoc = null; //svg document
var selectedNodes = [];
var pathNodes = []; //output of a pathfinding algorithm like bfs
var defaultNodeFill = 'white'; //may be overriden by hidden input element
var af; //adjacency file reset name

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


function updateInfo() {
    var info = '';
    for (var i=0; i<selectedNodes.length; ++i) {
        if (i==0) {
            info += '<b>Selected:</b><br>';
        }
        var node = selectedNodes[i];
        info += '<b>' + (i+1) + '</b>: ';
        info += nodeToString(node) + ' <b>Neighbors</b>: ';
        for (var j=0; j<node.neighbors.length; ++j) {
            var neighbor = nodeIdToNodeObjMap[node.neighbors[j]];
            /*
            var neighborFillColor = '#e0e0e0';
            setSvgElemFill(getSvgElemByNode(neighbor), neighborFillColor);
            */
            info += nodeToString(neighbor);
            if (j != node.neighbors.length-1) {
                info += ', ';
            }
        }
        info += '<br>';
    }
    $('#info').html(info);  
}

function selectNode(node) {
    var selectedNodeFill = $('#fill').val();
    var selectedNodeIdx = selectedNodes.indexOf(node);
    if (selectedNodeIdx != -1) {
        //unselect
        if (selectedNodes.length = 1) {
            selectedNodes.pop();
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
    updateInfo();
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

function ready() {
    af = $('#af').val();
    defaultNodeFill = $('#defaultNodeFill').val();
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
                console.log("failed to get node element");
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
                    alert("Greedy coloring failed. Insufficient number of colors.");
                    return;
                } else {
                    var color = remainingColors[0];
                    node.fill = color;
                    setSvgElemFill(getSvgElemByNode(node), color);
                }
            }
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
}
