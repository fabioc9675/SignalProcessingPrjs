// Cart Pole Simulation
// PID Controller
// Fabian Castano
// p5.js and Matter.js
// Matter library was added to index.html to use physics engine

// module aliases
const Engine = Matter.Engine;
const Render = Matter.Render;
const World = Matter.World;
const Bodies = Matter.Bodies;
const Body = Matter.Body;
const Constraint = Matter.Constraint;
const Vector = Matter.Vector;

// Matter Engine and World
let engine, world;

// Bodies in the world
let ground, cart, bob;

// Sliders for P, D and I gain
let pGainSlider, dGainSlider, iGainSlider;

// Previous angle to track angular velocity
let prevAngle = 0;
let intError = 0;

function setup() {
    createCanvas(600, 400).mousePressed(pushBob);

    buildWorld();
    buildCartPole();

    // Sliders for gain
    pGainSlider = createSlider(0, 0.01, 0.0, 0.001);
    createSpan("P Gain<br/>");
    dGainSlider = createSlider(0, 0.01, 0.0, 0.001);
    createSpan("D Gain<br/>");
    iGainSlider = createSlider(0, 0.01, 0.0, 0.001);
    createSpan("I Gain<br/>");

    createP("Click in the canvas to destabilize pendulum.");
    createButton("reset").mousePressed(function () {
        buildWorld();
        buildCartPole();
        intError = 0;
        error = 0;
    });
}

// Key presses to push on the bob and destabilize the pole
function pushBob() {
    let fx = 0.002;
    intError = 0;
    if (bob.position.x > mouseX) fx *= -1;
    const force = Vector.create(fx, 0);
    Body.applyForce(bob, bob.position, force);
}

// Function to build the world
function buildWorld() {
    // Create the world
    engine = Engine.create();
    world = engine.world;

    // Options for static boundaries
    const options = {
        friction: 0,
        restitution: 1.0,
        angle: 0,
        isStatic: true,
    };

    // The ground
    ground = Bodies.rectangle(width * 0.5, 295, width, 100, options);
    ground.w = width;
    ground.h = 100;
    World.add(world, ground);

    // Walls to block cart from leaving the canvas
    let rightWall = Bodies.rectangle(width, 150, 10, height, options);
    let leftWall = Bodies.rectangle(0, 150, 10, height, options);
    World.add(world, rightWall);
    World.add(world, leftWall);
}

// Function to build Cart Pole
function buildCartPole() {
    // Length of "pole"
    let restLength = 100;

    // Ther cart
    cart = Bodies.rectangle(width * 0.5, 240, 40, 20, {
        friction: 0,
        restitution: 0,
        angle: 0,
        isStatic: false,
    });
    cart.w = 40;
    cart.h = 20;
    World.add(world, cart);

    // Bob (attached to top of pole)
    bob = Bodies.circle(cart.position.x, cart.position.y - restLength, 10, {
        friction: 0,
        restitution: 0.5,
        angle: 0,
        isStatic: false,
    });
    bob.r = 10;
    World.add(world, bob);

    // The "pole" is a constrain between cart and bob
    const constrain = Constraint.create({
        bodyA: cart,
        bodyB: bob,
        length: restLength,
        stiffness: 1,
    });
    World.add(world, constrain);
}

function draw() {
    background(51);

    // Update the Engine
    Engine.update(engine);

    // What is pendulum arm angle?
    // TODO: is this a property that exist already in constraint object?
    let arm = createVector();
    arm.x = bob.position.x - cart.position.x;
    arm.y = bob.position.y - cart.position.y;
    let angle = arm.heading() + PI / 2;

    // Derivative of angle (angular velocity)
    let dt = 0.01;
    let angleV = (angle - prevAngle) / dt;
    // save previous angle
    prevAngle = angle;

    // PID Controller
    let error = 0 - angle;
    intError += error * dt;

    let pGain = 5 * pGainSlider.value();
    let dGain = 0.1 * dGainSlider.value();
    let iGain = iGainSlider.value();

    // TODO: why is pGain negative here? (flipped y axis?)
    // PID Controller
    let fx = -pGain * error + dGain * angleV - iGain * intError;
    const force = Vector.create(fx, 0);

    // Apply the force!
    Body.applyForce(cart, cart.position, force);

    // Draw the background
    noStroke();
    fill(112, 50, 126);
    rectMode(CENTER);
    rect(ground.position.x, ground.position.y, ground.w, ground.h);

    // Draw the cart
    noStroke();
    fill(45, 197, 244);
    rect(cart.position.x, cart.position.y, cart.w, cart.h);

    // Draw the pole
    stroke(255);
    strokeWeight(4);
    line(cart.position.x, cart.position.y, bob.position.x, bob.position.y);

    // Drawing the bob
    fill(240, 99, 164);
    circle(bob.position.x, bob.position.y, bob.r * 2);
}
