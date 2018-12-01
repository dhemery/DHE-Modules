#pragma once

namespace DHE {

class Trigger {
public:
  void step() {
    auto old_state = state;
    state = state_in();
    if (state != old_state)
      on_state_change(state);
  }

protected:
  virtual auto state_in() const -> bool = 0;
  virtual void on_rise() = 0;
  virtual void on_state_change(bool state) { on_rise(); }

private:
  bool state = false;
};

class Gate : public Trigger {
public:
  void on_state_change(bool state) override {
    if (state) {
      on_rise();
    } else {
      on_fall();
    }
  }

protected:
  virtual void on_fall() = 0;
};

class Mode {
public:
  virtual void enter(){};
  virtual void step(){};
  virtual void exit(){};
};
} // namespace DHE
