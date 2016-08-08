var canvas = document.getElementById('canvas');
var context = canvas.getContext('2d');
var setting = {'xmin': 0, 'ymin': 0, 'xmax': 1, 'ymax': 1};

function draw() {
  setting = {'xmin': 1e+20, 'ymin': 1e+20, 'xmax': -1e+20, 'ymax': -1e+20};
  var lines = document.getElementById('problem').value.split('\n');;
  var problem = parseProblem(lines);

  context.clearRect(0, 0, canvas.width, canvas.height);
  drawSilhouette(problem['silhouette']);
  drawSkeleton(problem['skeleton']);
}

function drawSilhouette(silhouette) {
  context.fillStyle = '#ee6666';
  for (var i = 0; i < silhouette.length; i++) {
    var polygon = silhouette[i];
    context.beginPath();
    var xy = convert(polygon[0]);
    context.moveTo(xy[0], xy[1]);
    for (var j = 1; j < polygon.length; j++) {
      var xy = convert(polygon[j]);
      context.lineTo(xy[0], xy[1]);
    }
    context.closePath();
    context.fill();
  }
}

function drawSkeleton(skeleton) {
  // set color
  for (var i = 0; i < skeleton.length; i++) {
    var line = skeleton[i];
    context.beginPath();
    var xy = convert(line[0]);
    context.moveTo(xy[0], xy[1]);
    xy = convert(line[1]);
    context.lineTo(xy[0], xy[1]);
    context.stroke();
  }
}

function convert(orig) {
  OFFSET = 50;
  SIZE = 200;

  var width = setting['xmax'] - setting['xmin'];
  var height = setting['ymax'] - setting['ymin'];
  var scale = SIZE / Math.max(Math.max(width, height), 1);

  var x = orig[0];
  var y = orig[1];
  x = (x - setting['xmin']) * scale + OFFSET;
  y = SIZE - (y - setting['ymin']) * scale + OFFSET;

  return [x, y];
}

function parseProblem(lines) {
  var line_id = 0;
  var problem = {'silhouette': [], 'skeleton': []};

  // parse silhouette
  var num_polygon = lines[line_id++];
  for (var i = 0; i < num_polygon; i++) {
    var num_vertex = lines[line_id++];
    var polygon = [];
    for (var j = 0; j < num_vertex; j++) {
      var vertex = parseVertex(lines[line_id++]);
      polygon.push(vertex);
    }
    problem['silhouette'].push(polygon);
  }

  // parse skeleton
  var num_line = lines[line_id++];
  for (var i = 0; i < num_line; i++) {
    var seg = lines[line_id++].split(' ');
    seg[0] = parseVertex(seg[0]);
    seg[1] = parseVertex(seg[1]);
    problem['skeleton'].push(seg);
  }

  return problem;
}

function parseVertex(val) {
  var vs = val.split(',');
  for (var i = 0; i < 2; i++) {
    if (vs[i].indexOf('/') > 0) {
      var f = vs[i].split('/');
      vs[i] = f[0] / f[1];
    } else {
      vs[i] = vs[i] - 0;
    }
  }

  setting['xmin'] = Math.min(setting['xmin'], vs[0]);
  setting['xmax'] = Math.max(setting['xmax'], vs[0]);
  setting['ymin'] = Math.min(setting['ymin'], vs[1]);
  setting['ymax'] = Math.max(setting['ymax'], vs[1]);

  return vs;
}
