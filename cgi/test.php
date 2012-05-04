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
</style>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
<script type="text/javascript">
var nodeList = []; //list of all node objects
var nodeIdToNodeObjMap = {}; //map of node id to node object (for fast lookups)
var nodeStringToNodeObjMap = {}; //map of node string to node object
var svg = null;
var svgdoc = null;
var selectedNode = null;

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
    var fips = fipsToString(node.nodeId);
    svgElem = svgdoc.getElementById(fips);
<?php } else { die("Unrecognized preset"); } ?>   
    return svgElem;
}

function setSvgElemFill(elem, fill) {
    $(elem).attr('fill', fill);
    $(elem).css('fill', fill);
}

function setDefaultSvgElemFill(elem) {
    var defaultFill = 'white';
<?php if ($af == "us-states") { ?>
    defaultFill = 'white';
<?php } else if ($af == "us-counties") { ?>
    defaultFill = '#d0d0d0';
<?php } else { die("Unrecognized preset"); } ?>   
    $(elem).attr('fill', defaultFill);
    $(elem).css('fill', defaultFill);   
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

function selectNode(node) {
    //unselect current selected node
    if (selectedNode != null) {
        currentSvgElem = getSvgElemByNode(selectedNode);
        setDefaultSvgElemFill(currentSvgElem);
        for (var i=0; i<selectedNode.neighbors.length; ++i) {
            var neighbor = nodeIdToNodeObjMap[selectedNode.neighbors[i]];
            setDefaultSvgElemFill(getSvgElemByNode(neighbor));
        }
    }
    //set new selected node and set fill
    selectedNode = node;
    var info = nodeToString(selectedNode);
    setSvgElemFill(getSvgElemByNode(node), $('#fill').val());
    info += '<br>Neighbors: ';
    for (var i=0; i<node.neighbors.length; ++i) {
        var neighbor = nodeIdToNodeObjMap[node.neighbors[i]];
        var neighborFillColor = 'gray';
        setSvgElemFill(getSvgElemByNode(neighbor), neighborFillColor);
        info += nodeToString(neighbor);
        if (i != node.neighbors.length-1) {
            info += ', ';
        }
    }
    $('#nodeInfo').html(info);  
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
            nodeIdToNodeObjMap[node.nodeId] = node;
            nodeStringToNodeObjMap[node.nodeString] = node;
            var svgElem = getSvgElemByNode(node);
            if (svgElem != null) {
                $(svgElem).click(function(){
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
                    $('#nodeInfo').text(node.nodeString + ' (' + node.nodeId + ')');
                });

                $(svgElem).mouseout(function(){
                    $('#nodeInfo').text('');
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
                $('#searchResults').html('');
                $('#search').val('');
            });
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
<span id="nodeInfo"></span>
<br/>
Fill: <input type="text" id="fill" value="red"/><br/>
Search: <input type="text" id="search" value=""/><br/>
<div id="searchResults"></div>
</body>
</html>
