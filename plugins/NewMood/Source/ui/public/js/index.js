// NewMood WebView UI - JUCE Integration
import * as Juce from "./juce/index.js";

// Parameter states
const parameterStates = {};

// Initialize UI when DOM is ready
document.addEventListener("DOMContentLoaded", () => {
    console.log("NewMood UI initializing...");
    initializeControls();
    setupJuceIntegration();
});

function initializeControls() {
    // Setup knobs
    const knobs = document.querySelectorAll('.knob');
    knobs.forEach(knob => {
        const param = knob.dataset.param;
        let isDragging = false;
        let startY = 0;
        let startValue = 0;
        
        knob.addEventListener('mousedown', (e) => {
            isDragging = true;
            startY = e.clientY;
            startValue = parseFloat(document.getElementById(`${param}-value`).textContent) / 100;
            e.preventDefault();
        });
        
        document.addEventListener('mousemove', (e) => {
            if (!isDragging) return;
            const delta = (startY - e.clientY) / 100;
            let newValue = Math.max(0, Math.min(1, startValue + delta));
            updateParameter(param, newValue);
        });
        
        document.addEventListener('mouseup', () => {
            isDragging = false;
        });
        
        // Double-click to reset
        knob.addEventListener('dblclick', () => {
            updateParameter(param, 0.5);
        });
    });
    
    // Setup buttons
    const buttons = document.querySelectorAll('.button');
    buttons.forEach(btn => {
        const param = btn.dataset.param;
        btn.addEventListener('click', () => {
            if (param === 'clear') {
                // Clear is a trigger
                Juce.setParameterValue(param, 1);
                setTimeout(() => Juce.setParameterValue(param, 0), 100);
            } else {
                // Toggle buttons
                const currentValue = parameterStates[param] || 0;
                const newValue = currentValue > 0.5 ? 0 : 1;
                updateParameter(param, newValue);
            }
        });
    });
    
    // Setup toggle switches
    const toggles = document.querySelectorAll('.toggle-switch');
    toggles.forEach(toggle => {
        const param = toggle.dataset.param;
        toggle.addEventListener('click', () => {
            const currentValue = parameterStates[param] || 0;
            const newValue = currentValue > 0.5 ? 0 : 1;
            updateParameter(param, newValue);
        });
    });
    
    // Setup subdivision buttons
    const subBtns = document.querySelectorAll('.sub-btn');
    subBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            const param = btn.dataset.param;
            const value = parseInt(btn.dataset.value);
            updateParameter(param, value);
            
            // Update visual state
            subBtns.forEach(b => b.classList.remove('active'));
            btn.classList.add('active');
        });
    });
}

function setupJuceIntegration() {
    // Wait for JUCE to be ready
    if (typeof Juce !== 'undefined') {
        Juce.init();
        console.log("JUCE integration ready");
        
        // Listen for parameter changes from C++
        Juce.onParameterChange = (paramId, value) => {
            parameterStates[paramId] = value;
            updateUI(paramId, value);
        };
    } else {
        console.warn("JUCE not available - running in preview mode");
    }
}

function updateParameter(param, value) {
    parameterStates[param] = value;
    updateUI(param, value);
    
    // Send to JUCE
    if (typeof Juce !== 'undefined') {
        Juce.setParameterValue(param, value);
    }
}

function updateUI(param, value) {
    // Update value display
    const valueEl = document.getElementById(`${param}-value`);
    if (valueEl) {
        valueEl.textContent = Math.round(value * 100);
    }
    
    // Update knob rotation
    const knobEl = document.getElementById(`${param}-knob`);
    if (knobEl) {
        const angle = -135 + (value * 270);
        knobEl.style.setProperty('--rotation', `${angle}deg`);
        knobEl.querySelector('::after')?.style.setProperty('transform', `rotate(${angle}deg) translateY(-17px)`);
    }
    
    // Update toggle state
    const toggleEl = document.getElementById(`${param}-toggle`);
    if (toggleEl) {
        toggleEl.classList.toggle('active', value > 0.5);
    }
    
    // Update button state
    const btnEl = document.getElementById(`${param}-btn`);
    if (btnEl) {
        btnEl.classList.toggle('active', value > 0.5);
    }
}