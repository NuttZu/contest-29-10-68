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

  saveState() {
    fs.writeFileSync(this.filePath, JSON.stringify(this.ledState, null, 2));
  }

  getState() {
    return this.ledState;
  }

  getLedState(ledName) {
    if (!(ledName in this.ledState)) {
      throw new Error(`❌ LED "${ledName}" not found`);
    }
    return this.ledState[ledName];
  }

  setLedState(ledName, newValue) {
    if (!(ledName in this.ledState)) {
      throw new Error(`❌ LED "${ledName}" not found`);
    }
    this.ledState[ledName] = newValue;
    this.saveState();
    return { [ledName]: newValue };
  }

  updateState(newState) {
    this.ledState = { ...this.ledState, ...newState };
    this.saveState();
    return this.ledState;
  }

  // ✅ Reset to default
  reset() {
    this.ledState = { ...this.defaultState };
    this.saveState();
    console.log("🔄 LED state reset to default");
    return this.ledState;
  }
}

class TempManager {
  constructor(filePath = "./tempData.json") {
    this.filePath = filePath;
    this.defaultData = {
      temp: 0,
      humid: 0,
    };
    this.data = this.loadOrCreateFile();
  }

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

  saveData() {
    fs.writeFileSync(this.filePath, JSON.stringify(this.data, null, 2));
  }

  getData() {
    return this.data;
  }

  getField(fieldName) {
    if (!(fieldName in this.data)) {
      throw new Error(`❌ Field "${fieldName}" not found`);
    }
    return this.data[fieldName];
  }

  setField(fieldName, newValue) {
    if (!(fieldName in this.data)) {
      throw new Error(`❌ Field "${fieldName}" not found`);
    }
    this.data[fieldName] = newValue;
    this.saveData();
    return { [fieldName]: newValue };
  }

  updateData(newData) {
    this.data = { ...this.data, ...newData };
    this.saveData();
    return this.data;
  }

  // ✅ Reset to default
  reset() {
    this.data = { ...this.defaultData };
    this.saveData();
    console.log("🔄 Temp data reset to default");
    return this.data;
  }
}

class VrStateManager {
  constructor(filePath = "./vrState.json") {
    this.filePath = filePath;
    this.defaultState = { vr: 0 };
    this.vrState = this.loadOrCreateFile();
  }

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

  saveState() {
    fs.writeFileSync(this.filePath, JSON.stringify(this.vrState, null, 2));
  }

  getState() {
    return this.vrState;
  }

  getVr() {
    return this.vrState.vr;
  }

  setVr(newValue) {
    this.vrState.vr = newValue;
    this.saveState();
    return { vr: newValue };
  }

  // ✅ Reset to default
  reset() {
    this.vrState = { ...this.defaultState };
    this.saveState();
    console.log("🔄 VR state reset to default");
    return this.vrState;
  }
}

class LightStateManager {
  constructor(filePath = "./lightState.json") {
    this.filePath = filePath;
    this.defaultState = { light: 0 };
    this.lightState = this.loadOrCreateFile();
  }

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

  saveState() {
    fs.writeFileSync(this.filePath, JSON.stringify(this.lightState, null, 2));
  }

  getState() {
    return this.lightState;
  }

  getLight() {
    return this.lightState.light;
  }

  setLight(newValue) {
    this.lightState.light = newValue;
    this.saveState();
    return { light: newValue };
  }

  // ✅ Reset to default
  reset() {
    this.lightState = { ...this.defaultState };
    this.saveState();
    console.log("🔄 Light state reset to default");
    return this.lightState;
  }
}

module.exports = {
  LedStateManager,
  TempManager,
  VrStateManager,
  LightStateManager,
};
