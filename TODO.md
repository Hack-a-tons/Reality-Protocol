# 🔮 Roadmap & Future Features

## Phase 1: Hackathon MVP (Current)
- [x] Basic Anchor-to-Tag Ranging (1-on-1)
- [x] LCD Dashboard for Distance visualization
- [ ] Integration with MPU6050 for "Shake/SOS" detection
- [ ] Connection to Postman for API reporting

## Phase 2: Swarm Intelligence
- [ ] **Dynamic Discovery:** Implement "Roll Call" protocol to auto-detect new tags entering the zone.
- [ ] **Mesh Relay:** Allow Tag B to report distance of Tag C back to Anchor A (Daisy Chaining).
- [ ] **Triangulation:** Use 3 Anchors to calculate X,Y,Z coordinates, not just raw distance.

## Phase 3: Context Engineering
- [ ] **Genetic Algorithm:** Feed false-positive sensor data into Lightning.ai to evolve the noise-filtering thresholds automatically.
- [ ] **Digital Twin:** Visualize the swarm topology in a Python Streamlit dashboard.
