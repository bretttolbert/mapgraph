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
$(function(){
    setTimeout(ready, 500);
});

function fipsToString(fips) {
    var str = fips.toString(10);
    while (str.length < 5) {
        str = '0' + str;
    }
    return str;
}

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
    $(elem).attr('fill', '#d0d0d0');
    $(elem).css('fill', '#d0d0d0');
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
                    if (selectedNode != null) {
                        currentSvgElem = getSvgElemByNode(selectedNode);
                        setDefaultSvgElemFill(currentSvgElem);
                    }
                    selectedNode = getNodeBySvgElemId(this.id);
                    $('#nodeInfo').text(selectedNode.nodeString 
                        + ' (' + selectedNode.nodeId + ')');
                    setSvgElemFill(this, $('#fill').val());
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
fill: <input type="text" id="fill" value="red"/>
</body>
</html>
