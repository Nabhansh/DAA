# Project 47: Satellite Image Tile Processing Pipeline Scheduler

## 1. Project Overview
- Explain what satellite image tiles are.
- Explain why processing jobs may depend on other jobs.
- State the goal: schedule jobs in an order that respects all dependencies.

## 2. Problem Statement
Describe the problem of arranging dependent processing jobs into a valid execution order. Explain what should happen if the dependencies contain a cycle or reference an unknown job.

## 3. Objectives
- Represent processing jobs and their dependencies.
- Implement Kahn’s topological sorting algorithm.
- Implement DFS-based topological sorting.
- Detect invalid dependencies and cycles.
- Test correctness and compare execution times.

## 4. Project Structure
Explain the purpose of each important directory and file:
- `include/` — header files and declarations.
- `src/` — scheduler implementations, main program, and benchmark.
- `tests/` — correctness tests.
- `data/jobs.txt` — sample input.
- `results/` — benchmark CSV files.
- `CMakeLists.txt` — build configuration.

## 5. Input Format
Document the input format used by `data/jobs.txt`. Explain the number of jobs, job ID, tile name, processing cost, dependency count, and dependency IDs.

Include a small example and explain what each line means.

## 6. Algorithms Used

### 6.1 Kahn’s Algorithm
Explain indegree, the queue, how jobs with no remaining prerequisites are selected, and how cycles are detected.

### 6.2 DFS-Based Topological Sort
Explain the three states: unvisited, visiting, and completed. Describe how the visiting state detects a cycle and how jobs are added to the result.

## 7. Pseudocode
Write your own pseudocode for both algorithms, matching your actual implementations.

## 8. Correctness
Explain why every dependency must appear before the job that depends on it. Explain how the program rejects cycles and invalid dependency references.

## 9. Complexity Analysis
Discuss time and space complexity using V for jobs and E for dependency edges. Explain the expected complexity of each implementation.

## 10. Testing
Document the test cases and their results. Record the actual output from your test run.

## 11. Benchmarking and Results
Explain how you generated test graphs, the job counts used, the dependency counts, and how runtime was measured. Include your median runtime comparison table.

## 12. Build and Run Instructions
Document the commands required to configure, build, test, run the sample input, and generate benchmark CSV output on Fedora/Linux.

## 13. Limitations and Future Improvements
Describe limitations of the current implementation and realistic improvements, such as repeated benchmark trials, more varied dependency graphs, or scheduling jobs across multiple processors.

## 14. Conclusion
Summarize what you implemented, tested, and learned from comparing the two algorithms.

## 15. References and Originality
List any sources you actually consulted and include the originality declaration required by your course instructions.
