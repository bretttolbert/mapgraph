<!DOCTYPE html>
<!--
File for experimenting with client side manipulation of SVG.
-->
<html>
<head>
<?php
$af = $_REQUEST["af"];
?>
<title>graphgame</title>
<style type="text/css">
body {
    font-family: sans-serif;
}
a:link {
    text-decoration: none;
    color: black;
}
a:visited {
    text-decoration: none;
    color: black;
}
a:hover {
    text-decoration: none;
    color: red;
}
a:active {
    text-decoration: none;
    color: red;
}
</style>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
<script type="text/javascript">
var nodeList = []; //list of all node objects
var nodeIdToNodeObjMap = {}; //map of node id to node object (for fast lookups)
var nodeStringToNodeObjMap = {}; //map of node string to node object
var svg = null;
var svgdoc = null;
var selectedNodes = [];
var pathNodes = []; //output of a pathfinding algorithm like bfs

<?php if ($af == "us-states") { ?>
var defaultNodeFill = 'white';
<?php } else if ($af == "us-counties") { ?>
var defaultNodeFill = '#d0d0d0';
<?php } else { die("Unrecognized preset"); } ?>   

function getNodeBySvgElemId(id) {
    var node = null;
<?php if ($af == "us-states") { ?>
    node = nodeStringToNodeObjMap[id];
<?php } else if ($af == "us-counties") { ?>
    var nodeId = parseInt(id, 10);
    node = nodeIdToNodeObjMap[nodeId];
<?php } ?>
    return node;
}

function getSvgElemByNode(node) {
    var svgElem = null;
<?php if ($af == "us-states") { ?>
    svgElem = svgdoc.getElementById(node.nodeString);
<?php } else if ($af == "us-counties") { ?>
    svgElem = svgdoc.getElementById(fipsToString(node.nodeId));
<?php } else { die("Unrecognized preset"); } ?>
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
<?php if ($af == "us-states") { ?>
    result += ' (' + node.nodeId + ')';
<?php } else if ($af == "us-counties") { ?>
    result += ' (' + fipsToString(node.nodeId) + ')';
<?php } else { die("Unrecognized preset"); } ?>   
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
        selectedNodes.splice(selectedNodeIdx, 1);
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

function clearPath() {
    for (var i in pathNodes) {
        var node = pathNodes[i];
        setSvgElemFill(getSvgElemByNode(node), defaultNodeFill);
    }
    pathNodes = [];
}

function bfs(startNode, goalNode, pathFill) {
    pathDisplayed = true;
    resetNodes();
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

function resetNodes() {
    for (var i=0; i<nodeList.length; ++i) {
        var node = nodeList[i];
        node.fill = defaultNodeFill;
        node.parent = null;
    }
}

function ready() {
    //$('#svg').attr('src', '../svg/USA_Counties_with_FIPS_and_names.svg');
    svg = document.getElementById('svg');
    svgdoc = svg.getSVGDocument();
    if (!svgdoc) {
        console.log('SVG Document Failed To Load');
        return;
    }

    $.getJSON(<?php echo "'graphgame.cgi?action=getNodes&af=$af'"?>, function(data) {
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
/*
                $(svgElem).mouseover(function(){
                    var node = null;
                <?php if ($af == "us-states") { ?>
                    node = nodeStringToNodeObjMap[this.id];
                <?php } else if ($af == "us-counties") { ?>
                    var nodeId = parseInt(this.id, 10);
                    node = nodeIdToNodeObjMap[nodeId];
                <?php } ?>
                    $('#info').text(node.nodeString + ' (' + node.nodeId + ')');
                });

                $(svgElem).mouseout(function(){
                    $('#info').text('');
                });
*/
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
        $('#bfs').click(function(){
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
    })
    .success(function() { console.log("second success"); })
    .error(function(jqXHR, textStatus, errorThrown) {
            console.log("error " + textStatus);
            console.log("incoming Text " + jqXHR.responseText);
        })
    .complete(function() { console.log("complete"); });
}
</script>
</head>
<body>
<!-- width="1110" height="704" -->
<?php if ($af == "us-states") { ?>
<embed id="svg" src="../svg/Map_of_USA_without_state_names.svg"></embed>
<?php } else if ($af == "us-counties") { ?>
<embed id="svg" src="../svg/USA_Counties_with_FIPS_and_names.svg"></embed>
<?php } ?>
<br/>
<span id="info"></span>
<br/>
Fill: <input type="text" id="fill" value="salmon"/>
 Search: <input type="text" id="search" value=""/><a href="#" id="clearSearch">X</a>
 <input type="button" id="random" value="Select Random"/>
 <input type="button" id="bfs" value="Breadth First Search"/>
<br/>
<div id="searchResults"></div>
</body>
</html>
