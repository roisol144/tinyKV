# tinykv design

Status: draft. Fill in each section before you build the matching milestone, and update it when reality disagrees.

## Goals
- A small embedded LSM-tree key-value store: `Put`, `Get`, `Delete`, later `Scan`.
- Crash safe: after `kill -9` at any point, reopening recovers every write that was acknowledged under the configured sync policy.
- Readable code over maximum speed. Benchmark against LevelDB/RocksDB to learn where the gap is, not to win.

## Non-goals
Transactions, replication, compression (initially), multi-process access.

## Architecture (to write)
```
Put -> WAL append (+fsync policy) -> memtable
memtable full -> immutable memtable -> flush -> L0 SSTable
Get -> memtable -> immutable memtable -> L0 (newest first) -> L1.. (binary search by key range)
background thread: compaction
```

## Decisions to make (write your answer and the reason)
1. **Memtable structure**: skip list vs std::map. Why?
2. **WAL record format**: length, CRC32C, type, payload. How do you detect a torn tail on recovery?
3. **Sync policy**: fsync per write, per batch, or never. What does each guarantee after power loss vs process crash?
4. **SSTable layout**: data blocks, index block, bloom filter, footer with offsets. Block size? Restart points for prefix compression?
5. **Sequence numbers and tombstones**: how does a delete shadow older versions in lower levels, and when can a tombstone be dropped?
6. **Compaction strategy**: leveled vs size-tiered. Write amplification vs read amplification vs space.
7. **Concurrency model**: single writer, many readers; how do readers get a consistent snapshot while a flush or compaction swaps files? (immutable versions + refcounts is the usual answer)
8. **Manifest**: how does the DB know which SSTables are live after a crash mid-compaction?

## Testing plan
- Unit tests per component (WAL, SSTable reader/writer, bloom filter, merge iterator).
- Model-based randomized test: apply the same random operations to tinykv and to `std::map`, compare after every step and after reopen.
- Crash test: child process writes in a loop, parent `kill -9`s it at random times, reopen and check invariants.
- Sanitizer runs (ASan/UBSan, TSan) in CI.

## References
- Designing Data-Intensive Applications, ch. 3
- Database Internals (Petrov), part 1
- LevelDB `doc/impl.md` and `doc/table_format.md`
- RocksDB wiki: MemTable, WAL, Compaction
