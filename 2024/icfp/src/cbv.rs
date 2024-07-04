// "CBV" stands for the Cult of the Bound Variable.
// cbv module plays a role to communicate with CBV.
// The communication is done in ICFP expression.
use reqwest;

const URL: &str = "http://localhost:8000/communicate";
const AUTH_TOKEN: &str = "00000000-0000-0000-0000-000000000000";

pub fn communicate(message: String) -> String {
    let client = reqwest::blocking::Client::new();
    let res = client
        .post(URL)
        .bearer_auth(AUTH_TOKEN)
        .body(message)
        .send()
        .unwrap();
    res.text().unwrap()
}
