# Low Latency Trading System

This repository contains the implementation and ongoing development of a **low latency trading system** written in modern C++.

The project began with a focus on a trade matching engine, but is now evolving into a broader trading infrastructure project with modular components for **matching, order book management, instrument configuration, validation, execution flow, and future risk, market data, and observability modules**.

The goal is to design, build, and iteratively optimize a system that reflects real-world trading infrastructure, with a strong focus on **performance, correctness, and systems-level design**.

---

## Tech Stack
- **Language:** C++20
- **Build System:** CMake
- **Core Libraries:** C++ Standard Library
- **Key Data Structures:** `std::vector`, `std::deque`, `std::list`, `std::map`, `std::unordered_map`
- **Development Environment:** Linux / WSL

---

## Current Status

The current implementation includes:

- Price-time priority (FIFO) matching
- Support for:
  - Limit orders
  - Market orders
  - Order cancellation
- Multiple order book implementations:
  - `ListOrderBook` (eager deletion using `std::list`)
  - `DequeOrderBook` (lazy deletion using `std::deque`)
- Per-fill execution tracking through fill events
- Trade generation and ID generation
- `BookManager` for symbol-indexed order book ownership
- `SymbolTable` and instrument configuration infrastructure
- Vectorized symbol-to-order-book/config management
- Matching logic templated on order book type

The system has recently been refactored so that **order books no longer own symbols directly**. Instead, symbol ownership and instrument metadata live at the management layer, creating a cleaner separation between per-symbol book logic and system-level routing/validation.

---

## Design Goals

This project is not just about correctness. It is about building a system that mirrors how **real trading systems** are engineered.

Key goals:

- Low latency execution
- Cache-aware data structures
- Deterministic behavior under load
- Clear separation of responsibilities across system components
- Ability to experiment with multiple order book implementations
- Modular architecture that can grow into a complete trading system

---

## Architecture Overview

```text
TradeEngine
├── BookManager
│   ├── SymbolTable
│   ├── InstrumentConfig
│   └── symbol_id -> OrderBook
├── MatchingEngine
├── ID Generators
└── Execution / Result Types
```

---

## Key Idea

The system uses **templated engine components** so different order book implementations can be swapped in without introducing runtime polymorphism overhead.

The architecture is moving toward a design where:

- `TradeEngine` acts as the top-level controller
- `BookManager` owns per-symbol books and configs
- `MatchingEngine` performs matching against a selected book
- Order books focus purely on book data structure behavior

---

## Order Book Implementations

### ListOrderBook

- Uses `std::list` per price level
- Immediate removal on cancel
- Clean matching loop with no inactive front elements
- Useful baseline for correctness and cancellation semantics

---

### DequeOrderBook

- Uses `std::deque` per price level
- Lazy deletion strategy:
  - Cancel = mark inactive
  - Match = clean up during traversal
- Better cancellation locality
- Useful for studying lazy cleanup vs eager deletion trade-offs

---

## Instrument and Symbol Infrastructure

The system now includes explicit infrastructure for:

- Symbol string -> symbol ID mapping
- Per-symbol instrument configuration
- Validation rules such as:
  - tick alignment
  - band/range checks
  - pre-match vs post-match validation semantics

This makes the project more realistic and lays the groundwork for future flat/indexed order book designs.

---

## Matching and Execution Model

The matching engine now produces **fill events for each actual match**, rather than only aggregated traded quantity.

This makes it possible to:

- track exact buy/sell order pairings
- generate trade records cleanly
- support partial fills across multiple resting orders and price levels
- aggregate executions later at the trade engine/result layer if desired

---

## Experimental Direction

A key focus of this project is **comparing order book designs** under realistic workloads.

This includes evaluating trade-offs in:

- Latency
- Memory access patterns
- Cache locality
- Cancellation efficiency
- Matching throughput
- Control-flow simplicity
- Data-structure behavior under realistic order flow

---

## Ongoing Work

This repository is actively evolving. Upcoming improvements include:

- `TradeEngine` implementation
- Updated tests for the refactored architecture
- Benchmarking and latency measurement
- Flat/indexed order book implementations
- Ring-buffer-based price-level designs
- Market data ingestion
- Risk engine
- Replay/simulation support
- Execution/result handling improvements
- Observability and profiling
- Networking and serialization

---

## Long Term Vision

The long-term goal is to evolve this repository into a **low-latency trading system** with modular infrastructure for:

- Matching Engine
- Multiple Order Book Designs
- Book Management / Instrument Metadata
- Market Data
- Risk Engine
- Replay / Simulation
- Networking
- Profiling / Observability
- Lock-free infrastructure where appropriate

The matching engine remains a core part of the system, but it is now one module inside a larger trading architecture.

---

## Notes

This is a systems-focused project prioritizing:

- Performance-aware design decisions
- Understanding of low-level behavior (cache, memory, branching)
- Practical trade-offs between abstraction and speed
- Realistic architectural evolution rather than isolated toy components

---

## Inspiration

This project draws inspiration from real-world trading systems and firms such as:

- Jane Street
- Hudson River Trading (HRT)
- Citadel Securities

---

**More to come...**

