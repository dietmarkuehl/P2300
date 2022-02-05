# P2300

This repository is for an implementation of the
[P2300](http://wg21.link/p2300) specification. As the
specification is currently still a bit in flux the [currently worked on
version](https://brycelelbach.github.io/wg21_p2300_std_execution/std_execution.html)
is used. The primary objective is to locate issues with the specification. Issues
can be reported on the [P2300 issue tracker](https://github.com/brycelelbach/wg21_p2300_std_execution/issues).

## Concept Overview

- `operation_state`

        template <class S>
        concept operation_state
            =  destructible<S>
            && is_object_v<S>
            && requires(S& s) { { <a href="#start-cpo">execution::start(s)</a> } noexcept; }
            ;

## Customization Point Overview

- <a name="set_error-cpo">`set_error(auto&& receiver, auto&& error) noexcept -> void`</a>
- <a name="set_stopped-cpo">`set_stopped(auto&& receiver) noexcept -> void`</a>
- <a name="start-cpo">`start(auto& state) noexcept -> void`</a>
