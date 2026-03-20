# Low Latency Trade Matching Engine
This repository contains the implementation and ongoing development of a **low latency trade matching engine** written in modern C++.

The goal of this project is to design, build, and iteratively optimize a matching engine that reflects real-world exchange systems, with a strong focus on **performance, correctness, and systems-level design**.

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
- Trade generation and tracking
- ID generation for orders and trades

The system is designed to allow plug-and-play order book strategies via a templated matching engine.

The matching logic is implemented with careful attention to:

- Predictable control flow
- Minimal branching in hot paths
- Clean separation between matching logic and data structure behavior

---

## Design Goals
This project is not just about correctness. It is about building a system that mirrors how **real trading systems** are engineered.

Key goals:
- Low latency execution
- Cache-aware data structures
- Deterministic behavior under load
- Clear separation of responsibilities (engine vs order book)
- Ability to experiment with multiple implementations

---

## Architecture Overview
```markdown
MatchingEngine 
├── OrderBook 
│   ├── Bids (price → FIFO queue) 
│   └── Asks (price → FIFO queue) 
├── ID Generators (Order / Trade) 
└── Trade Log (fulfilled orders)

```

### Key Idea
The matching engine is **templated on the order book type**, allowing different data structures to define behavior without introducing runtime overhead.

---

## Order Book Implementations

### ListOrderBook
- Uses `std::list` per price level
- Immediate removal on cancel
- Clean matching loop (no inactive orders)
- Higher cancellation cost due to deeper lookups

### DequeOrderBook
- Uses `std::deque` per price level

- **Lazy deletion strategy:**
  - Cancel = mark inactive
  - Match = clean up during traversal
- O(1) cancellation
- Slightly more complex matching loop

---

## Experimental Direction
A key focus of this project is **comparing order book designs under realistic workloads**.

This includes evaluating trade-offs in:
- Latency
- Memory access patterns
- Cancellation efficiency
- Matching throughput

---

## Ongoing Work
This repository is actively evolving. Upcoming improvements include:
- Unit and stress testing for correctness guarantees
- Benchmarking and latency measurement
- Alternative order book implementations:
  - Intrusive data structures
  - Cache-optimized layouts
- Replacing `std::map` with flat or indexed structures
- Memory optimization (custom allocators / arenas)

---

## Build & Run
(To be added as the project evolves)

---

## Notes
This is a systems-focused project, prioritizing:
- Performance-aware design decisions
- Understanding of low-level behavior (cache, memory, branching)
- Practical trade-offs between abstraction and speed

---

## Long Term Vision
The long-term goal is to evolve this into a **high-performance matching engine** approaching production-grade design, including:
- Lock-free components
- Multithreaded ingestion
- Advanced order types
- Real-time metrics and monitoring

---

## Inspiration
This project draws inspiration from real-world trading systems and firms such as:
- Jane Street
- Hudson River Trading (HRT)
- Citadel Securities

---

**More to come.....**
