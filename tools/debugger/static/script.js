/* SCREEN SWITCHING LOGIC */

const debugger_screen = document.getElementById("debugger-screen");
const connection_screen = document.getElementById("connection-screen");

function showConnectionScreen(){
    debugger_screen.classList.add("hidden");
    connection_screen.classList.remove("hidden");
}

function showDebugScreen(){
    debugger_screen.classList.remove("hidden");
    connection_screen.classList.add("hidden");
}

function startDebugSession(){
    // initialise all the frames
    for(let callback of init_callbacks){
        callback();
    }
    // update footer
    showDebugScreen();
}

const init_callbacks = [];

function registerSessionInit(callback){
    init_callbacks.push(callback);
}

function loaded(){
    showConnectionScreen();
}

document.getElementById("connection-form").addEventListener("submit", (e) => {
    e.preventDefault();
    const data = new FormData(e.target);
    const ip_addr = data.get("ip-address");

    openConnection(ip_addr);
})

// update footer's ip addr
registerSessionInit(() => {
    const field = document.getElementById("robot-ip");

    field.innerText = remote_address;
})

function endDebugSession(){
    closeConnection();
    showConnectionScreen();
}

loaded();

/* COMMUNICATION FUNCTIONS */
let connection_established = false;
let remote_address;

const socket = io();

// connection to proxy
socket.on("connect", () => {
    console.log("connected to proxy server");
});

socket.on("disconnect", () => {
    if(socket.active) return;
    alert("connection to the proxy server lost");
});

// communication with robot
socket.on("connection_report", (data) => {
    if(data.open){
        // connection opened
        if(!connection_established){
            connection_established = true;
            startDebugSession();
        } 
    }else{
        // connection closed
        connection_established = false;

        alert(
            "robot disconnected\n" +
            "reason: " + data.reason
        );
        showConnectionScreen();
    }
});

function send(data){
    socket.emit("send", {data: data});
}

function openConnection(address){
    remote_address = address;
    socket.emit("open", {addr: address});
}

function closeConnection(){
    connection_established = false;
    socket.emit("close");
}

socket.on("recv", (data) => {
    // deserealizing message
    const packet = deserializePacket(data["data"]);
    console.log(packet);
    // call corresponding functions depending on stream_id
    const stream = frame_recv_register[packet.stream_id];
    if(stream === undefined) return;
    const callbacks = stream[packet.func_id];
    if(callbacks === undefined) return;

    for(let callback of callbacks){
        callback(packet);
    }
});

function base64ToArrayBuffer(base64) {
    let binaryString = atob(base64);
    let bytes = new Uint8Array(binaryString.length);
    for (let i = 0; i < binaryString.length; i++) {
        bytes[i] = binaryString.charCodeAt(i);
    }
    return bytes.buffer;
}

function deserializePacket(data){
    const buffer = base64ToArrayBuffer(data);
    const view = new DataView(buffer);

    const stream_id = view.getUint8(0);
    const func_id = view.getUint8(1);
    const payload = buffer.slice(2);

    return {
        "stream_id": stream_id,
        "func_id": func_id,
        "payload": payload
    };
}

/* FRAMES LOGIC */

const INT_SIZE = 2;
const FLOAT_SIZE = 4;

/* callback register functions */
const frame_recv_register = {}

function registerDataRecv(stream_id, func_id, callback){
    if(frame_recv_register[stream_id] === undefined){
        frame_recv_register[stream_id] = {};
    }

    const stream = frame_recv_register[stream_id];
    if(stream[func_id] === undefined){
        stream[func_id] = new Set();
    }

    const callback_list = stream[func_id];
    callback_list.add(callback);
}

/* terminal frame */
registerSessionInit(termInit);

registerDataRecv(0, 0, termPrintRecv);

const terminal_output_container = document.getElementById("text-output-container");
const terminal_output = document.getElementById("text-output");

function termInit(){
    terminal_output.innerText = "";
}

const SCROLL_STICK_THRESHOLD = 10;

function termPrint(message){
    // check for autoscroll
    const scroll_pos = terminal_output_container.scrollTop;
    const scroll_bottom = terminal_output_container.scrollHeight + terminal_output_container.clientHeight
    const scroll_delta = scroll_bottom - scroll_pos;
    
    if(scroll_delta < SCROLL_STICK_THRESHOLD){
        // stick scroll
        terminal_output_container.scrollTo(0, scroll_bottom);
    }

    terminal_output.innerText += message + "\n";
}

function termPrintRecv(packet){
    const decoder = new TextDecoder();
    const message = decoder.decode(packet.payload);
    
    termPrint("> " + message);
}

const input_form = document.getElementById("terminal-input-form");
input_form.addEventListener("submit", (event) => {
    event.preventDefault();

    const data = new FormData(event.target);
    const message = data.get("message");

    // print data
    termPrint("< " + message);
    // clear field
    event.target.querySelector(":scope #text-input").value = "";

    // send message
    console.log("sending: " + message);
    send(message + "\n");
});


/* plot frame */
registerSessionInit(pltInit);

registerDataRecv(1, 0, pltPlotPointRecv);
registerDataRecv(1, 1, pltClearRecv);
registerDataRecv(1, 2, pltPlotObjectRecv);
registerDataRecv(1, 3, pltRemoveObjectRecv);

const canvas_element = document.getElementById("main-canvas");
const ctx = canvas_element.getContext("2d");

const object_prefab = document.getElementById("object-prefab");
const object_container = document.getElementById("object-overlay")
const POINT_SIZE = 2;

// IMPORTANT: we need to translate between coordinate systems
// y should be up
// x should be right
// angle should be CCW

const CANVAS_WIDTH = canvas_element.width;
const CANVAS_HEIGHT = canvas_element.height;

function pltInit(){
    visible_points = {};
    clearCanvas();

    object_container.innerHTML = "";
}

function clearCanvas(){
    ctx.clearRect(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);
}

function drawPointAt(x, y){
    ctx.fillRect((x + CANVAS_WIDTH/2) - POINT_SIZE/2, (CANVAS_HEIGHT/2 - y) - POINT_SIZE/2, POINT_SIZE, POINT_SIZE);
}

// group: list[point]
let visible_points = {};

const COLOR_PALLATE = ["green", "red", "blue", "black"];

function colorFromId(id){
    return COLOR_PALLATE[id % COLOR_PALLATE.length];
}

function drawPoints(){
    for(const group of Object.keys(visible_points)){
        const point_list = visible_points[group];
        if(point_list == undefined){
            continue;
        }
        ctx.fillStyle = colorFromId(group);

        for(const point of point_list){
            drawPointAt(point.x, point.y);
        }
    }
}

let redraw_pending = false;

function redraw(){
    if(redraw_pending) return;

    requestAnimationFrame((_) => {
        clearCanvas();
        drawPoints();

        redraw_pending = false;
    });
    redraw_pending = true;
}

function pltPlotPointRecv(packet){
    // deserealise params
    const view = new DataView(packet.payload);

    const x = view.getFloat32(0);
    const y = view.getFloat32(FLOAT_SIZE);
    const group = view.getUint16(FLOAT_SIZE*2);

    pltPlotPoint(x, y, group);
}
function pltPlotPoint(x, y, group){
    // add point
    if(visible_points[group] === undefined){
        visible_points[group] = [];
    }
    visible_points[group].push({
        "x": x,
        "y": y
    })

    redraw();
}

function pltClearRecv(packet){
    const view = new DataView(packet.payload);
    const group = view.getUint16(0);
    
    pltClear(group);
}

function pltClear(group){
    // delete specified group
    visible_points[group] = undefined;
    redraw();
}

function pltPlotObjectRecv(packet){
    const view = new DataView(packet.payload);
    const id = view.getUint16(0);
    const x = view.getFloat32(INT_SIZE);
    const y = view.getFloat32(INT_SIZE + FLOAT_SIZE);
    const angle = view.getFloat32(INT_SIZE + FLOAT_SIZE*2);

    pltPlotObject(id, x, y, angle);
}

function pltPlotObject(id, x, y, angle){
    let object = object_container.querySelector(":scope > .obj" + id);

    // no object found
    if(object === null){
        // instantiate new node
        object = object_prefab.cloneNode(true);
        // change class and id
        object.id = "";
        object.classList.add("obj"+id);
        // color object
        const path = object.querySelector(":scope #path1");
        path.style.fill = colorFromId(id);
        object_container.appendChild(object);
    }

    // translate object
    const obj_height = object.height.baseVal.value;
    const obj_width = object.width.baseVal.value;

    object.style.top = ((CANVAS_HEIGHT/2 - y) - obj_height/2) + "px";
    object.style.left = ((x + CANVAS_WIDTH/2) - obj_width/2) + "px";
    object.style.transform = `rotate(${-angle}rad)`;
}

function pltRemoveObjectRecv(packet){
    const view = new DataView(packet.payload);
    const id = view.getUint16(0);

    pltRemoveObject(id);
}

function pltRemoveObject(id){
    const objects = object_container.querySelectorAll(":scope > .obj" + id);
    objects.forEach((obj) => {obj.remove()});
}

/* parameter frame */
registerSessionInit(prtInit);

registerDataRecv(2, 0, prtUpdateRecv);
registerDataRecv(2, 1, prtProposeRecv);

const sensor_table = document.getElementById("sensors-table");
const sensor_tbody = sensor_table.querySelector("tbody");

function prtInit(){
    sensor_tbody.innerHTML = "";
}

function createRow(){
    const row = document.createElement("tr");
    
    sensor_tbody.appendChild(row);

    const id_cell = document.createElement("th");
    const name_cell = document.createElement("th");
    const value_cell = document.createElement("th");

    id_cell.classList.add("id-cell");
    name_cell.classList.add("name-cell");
    value_cell.classList.add("value-cell");

    row.appendChild(id_cell);
    row.appendChild(name_cell);
    row.appendChild(value_cell);
    
    return row;
}

function getRowCells(row){
    return {
        "id_cell": row.querySelector(":scope .id-cell"),
        "name_cell": row.querySelector(":scope .name-cell"),
        "value_cell": row.querySelector(":scope .value-cell")
    };
}

function getOrCreateRow(id){
    const entries = sensor_tbody.querySelectorAll(":scope tr.entry"+id);
    
    // delete any duplicates
    entries.forEach((node, idx, parent) => {
        if(idx == 0) return;

        node.remove();
    })

    let entry = entries[0];
    // if there are no entries
    if(entry == null){
        entry = createRow();
        entry.classList.add("entry"+id);

        const cells = getRowCells(entry);
        cells.id_cell.innerText = id;
        cells.name_cell.innerText = "-";
        cells.value_cell.innerText = "N/A";
    }

    return entry;
}

function prtUpdateRecv(packet){
    const view = new DataView(packet.payload);

    const id = view.getUint16();
    const value = view.getFloat32(INT_SIZE);

    prtUpdate(id, value);
}

function prtUpdate(id, value){
    const row = getOrCreateRow(id);
    const cells = getRowCells(row);

    cells.value_cell.innerText = value;
}

function prtProposeRecv(packet){
    const view = new DataView(packet.payload);

    const id = view.getUint16();

    const text_buffer = packet.payload.slice(INT_SIZE);
    const name = decoder.decode(text_buffer);

    prtPropose(id, name);
}

function prtPropose(id, name){
    const row = getOrCreateRow(id);
    const cells = getRowCells(row);

    cells.name_cell.innerText = name;
}