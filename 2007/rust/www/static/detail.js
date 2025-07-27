function get_entry() {
    let url = new URL(window.location.href);
    let params = url.searchParams;
    let prefix = params.get('prefix');
    console.log(prefix);

    fetch("/api/get_entry", {
        method: "post",
        body: JSON.stringify({ 'prefix': prefix })
    })
        .then(async response => {
            const data = await response.json();
            update_html(data);
        })

}

async function update_html(data) {
    // TODO: Error handling

    data['hash'] = await sha256(data['prefix']);

    update_prefix(data);
    update_timestamp(data);
    update_picture(data);
    update_rna(data);
    update_diff(data);
    update_risk(data);
    update_time(data);
}

function update_timestamp(data) {
    const date_time = new Date(data['timestamp'] * 1000);
    const date_str = date_time.toLocaleString('ja-JP', { 'dateStyle': 'long', 'timeStyle': 'medium', 'hour12': false });
    document.getElementById('timestamp').innerHTML = date_str;
}

function update_picture(data) {
    const hash = data['hash'];
    var html = '<img src="/static/images/' + hash + '.png">';
    document.getElementById('picture').innerHTML = html;
}

function update_prefix(data) {
    const prefix = data['prefix'];
    const html = "<code>" + prefix + "</code>";
    document.getElementById('prefix').innerHTML = html;

    process_prefix(prefix);
}

function update_rna(data) {
    let rna = data['rna_size'];
    document.getElementById('rna').innerHTML = rna;
}

function update_diff(data) {
    let diff = data['diff'];
    document.getElementById('num_diff').innerHTML = diff;
}

function update_risk(data) {
    let pre = data['prefix'].length;
    let diff = data['diff'];
    document.getElementById('risk').innerHTML = 10 * diff + pre;
}

function update_time(data) {
    const t = data['elapsed_time'];
    document.getElementById('time').innerHTML = t.toFixed(2) + " sec.";
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

function process_prefix(dna) {
    let pattern = '';
    let lvl = 0;
    while (true) {
        if (dna.startsWith('C')) {
            dna = dna.substring(1);
            pattern += '<u>I</u>';
        } else if (dna.startsWith('F')) {
            dna = dna.substring(1);
            pattern += '<u>C</u>';
        } else if (dna.startsWith('P')) {
            dna = dna.substring(1);
            pattern += '<u>F</u>';
        } else if (dna.startsWith('IC')) {
            dna = dna.substring(2);
            pattern += '<u>P</u>';
        } else if (dna.startsWith('IP')) {
            dna = dna.substring(2);
            const nat_result = nat(dna);
            const n = nat_result[0];
            dna = nat_result[1];
            pattern += '!<sub>' + n + '</sub>';
        } else if (dna.startsWith('IF')) {
            dna = dna.substring(3);
            const consts_result = process_consts(dna);
            const s = consts_result[0];
            dna = consts_result[1];
            pattern += '?<sub>' + s + '</sub>';
        } else if (dna.startsWith('IIP')) {
            dna = dna.substring(3);
            lvl += 1;
            pattern += '(';
        } else if (dna.startsWith('IIF') || dna.startsWith('IIC')) {
            dna = dna.substring(3);
            if (lvl == 0) {
                break;
            }
            lvl -= 1;
            pattern += ')';
        } else if (dna.startsWith('III')) {
            dna = dna.substring(10);
        } else { break; }
    }
    console.log('Pattern: ' + pattern);

    let template = '';
    while (true) {
        if (dna.startsWith('C')) {
            dna = dna.substring(1);
            template += '<u>I</u>';
        } else if (dna.startsWith('F')) {
            dna = dna.substring(1);
            template += '<u>C</u>';
        } else if (dna.startsWith('P')) {
            dna = dna.substring(1);
            template += '<u>F</u>';
        } else if (dna.startsWith('IC')) {
            dna = dna.substring(2);
            template += '<u>P</u>';
        } else if (dna.startsWith('IF') || dna.startsWith('IP')) {
            dna = dna.substring(2);
            const nat_result_n = process_nat(dna);
            const n = nat_result_n[0];
            const nat_result_l = process_nat(nat_result_n[1]);
            const l = nat_result_l[0];
            dna = nat_result_l[1];
            template += '' + n + '<sub>' + l + '</sub>';
        } else if (dna.startsWith('IIF') || dna.startsWith('IIC')) {
            dna = dna.substring(3);
            break;
        } else if (dna.startsWith('IIP')) {
            dna = dna.substring(3);
            const nat_result = process_nat(dna);
            const n = nat_result[0];
            dna = nat_result[1];
            template += '|' + n + '|';
        } else if (dna.startsWith('III')) {
            dna = dna.substring(10);
        } else { break; }
    }

    document.getElementById('pattern').innerHTML = pattern + ' / ' + template + ' / ' + '<code>' + dna + '</code>';
}

function process_nat(dna) {
    let i = 0;
    while (dna.charAt(i) != 'P') {
        i++;
    }
    let next_dna = dna.substring(i + 1);

    let n = 0;
    --i;
    while (i >= 0) {
        if (dna.charAt(i) == 'C') {
            n = 2 * n + 1;
        } else {
            n = 2 * n;
        }
        --i;
    }
    return [n, next_dna];
}

function process_consts(dna) {
    let s = '';
    while (true) {
        if (dna.startsWith('C')) {
            dna = dna.substring(1);
            s += 'I';
        } else if (dna.startsWith('F')) {
            dna = dna.substring(1);
            s += 'C';
        } else if (dna.startsWith('P')) {
            dna = dna.substring(1);
            s += 'F';
        } else if (dna.startsWith('IC')) {
            dna = dna.substring(2);
            s += 'P';
        } else { break; }
    }
    return [s, dna];
}