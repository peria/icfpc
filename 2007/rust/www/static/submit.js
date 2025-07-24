async function submit(params) {
    const input_field = document.getElementById('prefix');
    var prefix = validate(input_field.value);
    input_field.value = prefix;

    console.log("Execute prefix: " + prefix);
    console.log("JSON: " + JSON.stringify({ prefix: prefix }));

    try {
        const response = await fetch("./execute", {
            method: "post",
            body: JSON.stringify({ prefix: prefix }),
        })
            .then(result => { });
    } catch { }
}

function validate(prefix) {
    return prefix.replace(/([^ICFP]+)/g, '');
}
