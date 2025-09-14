#pragma once

#include <memory>
#include <functional>

// General case: In -> Out
template <typename In, typename Out>
class Action {
private:
  std::shared_ptr<std::function<Out(In)>> fn;

public:
  inline Out run(In arg) const {
    return (*fn)(arg);
  }

  template <typename NextOut>
  Action<In, NextOut> then(std::function<NextOut(Out)> f) const {
    auto prevFn = fn;
    return Action<In, NextOut>([prevFn, f](In arg) {
      return f((*prevFn)(arg));
    });
  }

  // Chain into void
  Action<In, void> then(std::function<void(Out)> f) const {
    auto prevFn = fn;
    return Action<In, void>([prevFn, f](In arg) {
      f((*prevFn)(arg));
    });
  }

  Action(const std::function<Out(In)> &f)
    : fn(std::make_shared<std::function<Out(In)>>(f)) {}
};

// Case: In -> void
template <typename In>
class Action<In, void> {
private:
  std::shared_ptr<std::function<void(In)>> fn;

public:
  inline void run(In arg) const {
    (*fn)(arg);
  }

  Action<In, void> then(std::function<void(In)> f) const {
    auto prevFn = fn;
    return Action<In, void>([prevFn, f](In arg) {
      (*prevFn)(arg);
      f(arg);
    });
  }

  Action(const std::function<void(In)> &f)
    : fn(std::make_shared<std::function<void(In)>>(f)) {}
};

// Case: void -> Out
template <typename Out>
class Action<void, Out> {
private:
  std::shared_ptr<std::function<Out()>> fn;

public:
  inline Out run() const {
    return (*fn)();
  }

  template <typename NextOut>
  Action<void, NextOut> then(std::function<NextOut(Out)> f) const {
    auto prevFn = fn;
    return Action<void, NextOut>([prevFn, f]() {
      return f((*prevFn)());
    });
  }

  Action(const std::function<Out(void)> &f)
    : fn(std::make_shared<std::function<Out(void)>>(f)) {}
};

// Case: void -> void
template <>
class Action<void, void> {
private:
  std::shared_ptr<std::function<void()>> fn;

public:
  inline void run() const {
    (*fn)();
  }

  inline Action<void, void> then(const std::function<void()> &f) const {
    auto prevFn = fn;
    return Action<void, void>([prevFn, f]() {
      (*prevFn)();
      f();
    });
  }

  inline Action(const std::function<void()> &f)
    : fn(std::make_shared<std::function<void()>>(f)) {}

  inline Action(const std::shared_ptr<std::function<void()>> &f)
    : fn(f) {}
};
