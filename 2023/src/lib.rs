mod base;
mod image;
mod problem;
mod solution;

pub use crate::image::Image;
pub use base::{Musician, Placement, Rect};
pub use problem::Problem;
pub use solution::Solution;

pub fn add(left: usize, right: usize) -> usize {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}
