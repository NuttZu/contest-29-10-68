const fs = require("fs");

class LedStateManager {
  constructor(filePath = "./ledState.json") {
    this.filePath = filePath;
    this.defaultState = {
      led1State: false,
      led2State: false,
      led3State: false,
      led4State: false,
    };
    this.ledState = this.loadOrCreateFile();
  }

  // Load existing file or create new one
  loadOrCreateFile() {
    if (!fs.existsSync(this.filePath)) {
      fs.writeFileSync(this.filePath, JSON.stringify(this.defaultState, null, 2));
      console.log("✅ Created default LED state file");
      return { ...this.defaultState };
    } else {
      const data = JSON.parse(fs.readFileSync(this.filePath, "utf8"));
      console.log("✅ Loaded existing LED state file");
      return data;
    }
  }

  // Save current state
  saveState() {
    fs.writeFileSync(this.filePath, JSON.stringify(this.ledState, null, 2));
  }

  // Get all states
  getState() {
    return this.ledState;
  }

  // Get single LED state
  getLedState(ledName) {
    if (!(ledName in this.ledState)) {
      throw new Error(`❌ LED "${ledName}" not found`);
    }
    return this.ledState[ledName];
  }

  // Update one LED only
  setLedState(ledName, newValue) {
    if (!(ledName in this.ledState)) {
      throw new Error(`❌ LED "${ledName}" not found`);
    }
    this.ledState[ledName] = newValue;
    this.saveState();
    return { [ledName]: newValue };
  }

  // Update multiple LEDs at once
  updateState(newState) {
    this.ledState = { ...this.ledState, ...newState };
    this.saveState();
    return this.ledState;
  }
}

module.exports = LedStateManager;
