const canvas = document.getElementById('picture');
const canvas_context = canvas.getContext('2d');
const main_canvas = canvas_context.createImageData(canvas.width, canvas.height);

const pixel_to_galaxy = (px, py) => {
    return [Math.floor((px + canvas.offset_x) / canvas.scale),
	    Math.floor((py + canvas.offset_y) / canvas.scale)];
};

const clear_canvas = () => {
    // canvas_context.fillStyle = "rgba(0, 0, 0, 1)";
    // canvas_context.clearRect(0, 0, main_canvas.width, main_canvas.height);
    // canvas_context.rect(0, 0, main_canvas.width, main_canvas.height);
    // canvas_context.fill();
};

const draw_picture = (json_picture) => {
    pictures = json_picture["picture"]
    pictures.forEach(function (picture) {
	var image_data = canvas_context.createImagedata(main_canvas);
	const offx = main_canvas.width / 2;
	const offy = main_canvas.height / 2;
	image_data.translate(offx, offy);
	image_data.fillStyle = "rgba(0, 0, 0, 0.9)";
	image_data.clearRect(0, 0, image_data.width, image_data.height);
	image_data.fillStyle = "rgba(255, 255, 255, 0.9)";
	picture.forEach(function(xy) {
	    const [x, y] = xy;
	    image_data.fillRect(x, y, 1, 1);
	});
	main_canvas.drawImage(image_data, offx, offy);
    });
    draw_pixels(pixels);
};

const click_picture = (gx, gy) => {
    // update_status_waiting();
    $.getJSON("/click", { "x": gx, "y": gy }, function (json_picture) {
	draw_picture(json_picture);
    });
};

$(function() {
    // Add as new properties
    canvas.scale = 3;
    canvas.galaxy_width = canvas.width() / canvas.scale;
    canvas.galaxy_height = canvas.height() / canvas.scale;
    canvas.offset_x = -canvas.width() / 2;  // in px
    canvas.offset_y = -canvas.height() / 2;  // in px
    
    $(document).ready(function() {
	clear_canvas();
    });

    $('#picture').click(ev => {
	const rect = canvas.offset();
	const [gx, gy] = pixel_to_galaxy(ev.pageX - rect.left, ev.pageY - rect.top);
	console.log("click (" + gx + ", " + gy + ")");
	click_picture(gx, gy);
    });
});
