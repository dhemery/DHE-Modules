#pragma once

namespace DHE {

class Trigger {
public:
  void step() {
    auto old_state = state;
    state = trigger_in();
    if (state == old_state)
      return;
    if (state) {
      on_rise();
    }
  }
  void set() { state = true; }
  void reset() { state = false; }

protected:
  virtual auto trigger_in() const -> bool = 0;
  virtual void on_rise() = 0;

private:
  bool state = false;
};

class Gate {
public:
  void step() {
    auto old_state = state;
    state = gate_in();
    if (state == old_state)
      return;
    if (state) {
      on_rise();
    } else {
      on_fall();
    }
  }
  void set() { state = true; }
  void reset() { state = false; }

protected:
  virtual auto gate_in() const -> bool = 0;
  virtual void on_rise() = 0;
  virtual void on_fall() = 0;

private:
  bool state = false;
};

class Mode {
public:
  virtual void enter(){};
  virtual void step(){};
  virtual void exit(){};
};
} // namespace DHE
