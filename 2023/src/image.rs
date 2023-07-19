use crate::{Point, Rect};

use opencv as cv;

pub struct Image {
    _size: Rect,
    canvas: opencv::core::Mat,
}

impl Image {
    pub fn new(size: &Rect) -> Image {
        let canvas = opencv::core::Mat::new_rows_cols_with_default(
            size.height() as i32,
            size.width() as i32,
            opencv::core::CV_8UC3,
            cv::core::Scalar::from((128.0, 128.0, 128.0)),
        )
        .unwrap();
        Image {
            _size: *size,
            canvas,
        }
    }

    pub fn save(&self, path: &str) {
        let params = cv::core::Vector::new();
        cv::imgcodecs::imwrite(path, &self.canvas, &params).unwrap();
    }

    pub fn circle(&mut self, center: &Point, radius: f64, color: (f64, f64, f64)) {
        let cx = center.x as i32;
        let cy = center.y as i32;
        cv::imgproc::circle(
            &mut self.canvas,
            cv::core::Point::from((cx, cy)),
            radius as i32,
            cv::core::Scalar::from(color),
            cv::imgproc::FILLED,
            cv::imgproc::LINE_8,
            0,
        )
        .unwrap();
    }

    pub fn rectangle(&mut self, rect: &Rect, color: (f64, f64, f64)) {
        let rect = cv::core::Rect::from((
            cv::core::Point::from((rect.left as i32, rect.bottom as i32)),
            cv::core::Point::from((rect.right as i32, rect.top as i32)),
        ));
        cv::imgproc::rectangle(
            &mut self.canvas,
            rect,
            cv::core::Scalar::from(color),
            cv::imgproc::FILLED,
            cv::imgproc::LINE_8,
            0,
        )
        .unwrap();
    }
}
