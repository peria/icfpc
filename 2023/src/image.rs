use crate::{Placement, Rect};

struct Image {
    size: Rect,
    canvas: opencv::core::Mat,
}

impl Image {
    pub fn new(size: Rect) -> Image {
        let mut canvas = opencv::core::Mat::new_rows_cols_with_default(
            size.height() as i32,
            size.width() as i32,
            opencv::core::CV_8UC3,
            opencv::core::Scalar::from((0.5, 0.5, 0.5)),
        )
        .unwrap();

        Image { size, canvas }
    }

    pub fn circle(&mut self, center: &Placement, _radius: f64) {
        let cx = center.x;
        let cy = center.y;
        // drawing::draw_filled_circle_mut(&mut self.image);
    }

    pub fn rectangle(&mut self, rect: &Rect) {}
}
