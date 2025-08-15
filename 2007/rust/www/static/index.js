row_keys = []
processing_keys = new Set();

function setup() {
    document.getElementById('page').addEventListener('input', ev => {
        let data = ev.data - 0;
        // TODO: Update prefix to set the catalog page
        
    });

    get_all();
}

async function submit(params) {
    const input_field = document.getElementById('prefix');
    var prefix = validate(input_field.value);
    input_field.value = prefix;

    console.log("Execute: " + prefix);
    try {
        fetch("/execute", {
            method: "post",
            body: JSON.stringify({ prefix: prefix }),
        });
        await reload_async(prefix);
    } catch (e) {
        console.log("Error in execute: " + e);
    }
}

function validate(prefix) {
    return prefix.replace(/([^ICFP]+)/g, '');
}

function get_all() {
    fetch("/api/get_all", {
        method: "post",
        body: ''
    })
        .then(async response => {
            var data = await response.json();
            for (const row of Object.values(data)) {
                maybe_add_table_row_async(row);
            }
        })
}

async function reload_async(prefix) {
    const hash = await sha256(prefix);
    if (hash in row_keys) {
        return;
    }

    processing_keys.add(hash);
    fetch("/api/get_entry", {
        method: "post",
        body: JSON.stringify({ 'prefix': prefix })
    })
        .then(async response => {
            const data = await response.json();
            maybe_add_table_row_async(data);
        })
}

async function maybe_add_table_row_async(data) {
    const prefix = data['prefix'];
    const hash = await sha256(prefix);
    if (row_keys.includes(hash)) {
        // do nothing
    } else if ('is_processing' in data) {
        setTimeout(async () => reload_async(prefix), 1000);
    } else {
        processing_keys.delete(hash);
        data['hash'] = hash;
        add_table_row(data);
    }
}

async function sha256(message) {
    const msg_u8 = new TextEncoder("utf-8").encode(message);
    const buffer = await crypto.subtle.digest('SHA-256', msg_u8);
    const hash_array = Array.from(new Uint8Array(buffer));
    const hash = hash_array
        .map((b) => b.toString(16).padStart(2, '0'))
        .join('');
    return hash;
}

function add_table_row(data) {
    const hash = data['hash'];
    row_keys.push(hash);

    var row = document.createElement('tr');
    var html = '';
    html += "<td>" + html_timestamp(data);
    html += "<td>" + html_picture(data);
    html += "<td>" + html_prefix(data);
    html += "<td>" + html_rna(data);
    html += "<td>" + html_diff(data);
    html += "<td>" + html_risk(data);
    html += "<td>" + html_time(data);
    row.innerHTML = html;

    var table = document.getElementById('table');
    table.appendChild(row);
}

function html_timestamp(data) {
    const date_time = new Date(data['timestamp'] * 1000);
    return date_time.toLocaleString();
}

function html_picture(data) {
    const hash = data['hash'];
    var html = '<img src="/static/images/' + hash + '.png" width=100>';
    html = '<a href="/detail?prefix=' + data['prefix'] + '">' + html + '</a>';
    return html;
}

function html_prefix(data) {
    return data['prefix'];
}

function html_rna(data) {
    return data['rna_size'];
}

function html_diff(data) {
    return data['diff'];
}

function html_risk(data) {
    let pre = data['prefix'].length;
    let diff = data['diff'];
    return 10 * diff + pre;
}

function html_time(data) {
    const t = data['elapsed_time'];
    return t.toFixed(2) + " sec.";
}
