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

class TempManager {
  constructor(filePath = "./tempData.json") {
    this.filePath = filePath;
    this.defaultData = {
      temp: 0,
      humid: 0
    };
    this.data = this.loadOrCreateFile();
  }

  // Load existing file or create new one
  loadOrCreateFile() {
    if (!fs.existsSync(this.filePath)) {
      fs.writeFileSync(this.filePath, JSON.stringify(this.defaultData, null, 2));
      console.log("✅ Created default tempData.json file");
      return { ...this.defaultData };
    } else {
      const data = JSON.parse(fs.readFileSync(this.filePath, "utf8"));
      console.log("✅ Loaded existing tempData.json file");
      return data;
    }
  }

  // Save current data to file
  saveData() {
    fs.writeFileSync(this.filePath, JSON.stringify(this.data, null, 2));
  }

  // Get all data
  getData() {
    return this.data;
  }

  // Get a single field (temp or humid)
  getField(fieldName) {
    if (!(fieldName in this.data)) {
      throw new Error(`❌ Field "${fieldName}" not found`);
    }
    return this.data[fieldName];
  }

  // Set a single field
  setField(fieldName, newValue) {
    if (!(fieldName in this.data)) {
      throw new Error(`❌ Field "${fieldName}" not found`);
    }
    this.data[fieldName] = newValue;
    this.saveData();
    return { [fieldName]: newValue };
  }

  // Update multiple fields at once
  updateData(newData) {
    this.data = { ...this.data, ...newData };
    this.saveData();
    return this.data;
  }
}

class VrStateManager {
  constructor(filePath = "./vrState.json") {
    this.filePath = filePath;
    this.defaultState = { vr: 0 };
    this.vrState = this.loadOrCreateFile();
  }

  // Load existing file or create new one
  loadOrCreateFile() {
    if (!fs.existsSync(this.filePath)) {
      fs.writeFileSync(this.filePath, JSON.stringify(this.defaultState, null, 2));
      console.log("✅ Created default VR state file");
      return { ...this.defaultState };
    } else {
      const data = JSON.parse(fs.readFileSync(this.filePath, "utf8"));
      console.log("✅ Loaded existing VR state file");
      return data;
    }
  }

  // Save current state
  saveState() {
    fs.writeFileSync(this.filePath, JSON.stringify(this.vrState, null, 2));
  }

  // Get all states
  getState() {
    return this.vrState;
  }

  // Get VR value
  getVr() {
    return this.vrState.vr;
  }

  // Update VR value
  setVr(newValue) {
    this.vrState.vr = newValue;
    this.saveState();
    return { vr: newValue };
  }
}

class LightStateManager {
  constructor(filePath = "./lightState.json") {
    this.filePath = filePath;
    this.defaultState = { light: 0 };
    this.lightState = this.loadOrCreateFile();
  }

  // Load existing file or create new one
  loadOrCreateFile() {
    if (!fs.existsSync(this.filePath)) {
      fs.writeFileSync(this.filePath, JSON.stringify(this.defaultState, null, 2));
      console.log("✅ Created default Light state file");
      return { ...this.defaultState };
    } else {
      const data = JSON.parse(fs.readFileSync(this.filePath, "utf8"));
      console.log("✅ Loaded existing Light state file");
      return data;
    }
  }

  // Save current state
  saveState() {
    fs.writeFileSync(this.filePath, JSON.stringify(this.lightState, null, 2));
  }

  // Get all states
  getState() {
    return this.lightState;
  }

  // Get light value
  getLight() {
    return this.lightState.light;
  }

  // Update light value
  setLight(newValue) {
    this.lightState.light = newValue;
    this.saveState();
    return { light: newValue };
  }
}

module.exports = { 
    LedStateManager,  
    TempManager, 
    VrStateManager, 
    LightStateManager 
};
