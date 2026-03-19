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
- An `std::list` Order book structure with:
    - Bid/ask price levels
    - Per-level FIFO queues
- Trade generation and tracking
- ID generation for orders and trades

The matching logic is implemented with careful attention to:

- predictable control flow
- minimal branching in hot paths
- clean separation between matching engine and order book

---

## Design Goals
This project is not just about correctness. It is about building a system that mirrors how **real trading systems** are engineered.

Key goals:
- Low latency execution
- Cache-friendly data structures (future work)
- Deterministic behavior under load
- Clear separation of responsibilities (engine vs order book)
- Incremental optimization from a correct baseline

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

---

## Ongoing Work
This repository is actively evolving. Upcoming improvements include:
- Unit and stress testing for correctness guarantees
- Benchmarking and latency measurement
- Alternative order book implementations:
  - `std::deque`-based queues
  - Intrusive data structures
  - Cache-optimized layouts
- Replacing `std::map` with flat or indexed structures
- Exploring lazy cancellation vs direct removal
- Memory optimization (custom allocators / arenas)

--- 

## Experimental Direction
A key focus of this project is **prototyping multiple order book designs** and evaluating their trade-offs in:
- Latency
- Memory access patterns
- Cancellation efficiency
- Scalability

This will allow direct comparison of different approaches used in real-world trading systems.

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
