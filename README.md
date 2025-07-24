# FileInSight

**FileInSight** is a file management and forensic analysis tool implemented in modern C++.  
It combines multiple features such as compression, encryption, file recovery, duplicate scanning, similarity detection, and more – all in one CLI application.

## 🔧 Features

- 🔍 **File Analytics**
    - Entropy analysis
    - Magic-Byte-Scanner to identify file types
    - Path analyzing
    - Age finder (last used > x days ago)

- 🧠 **Similarity and Duplicate Finder**
    - Hashing-based Duplicate Scanner
    - **TLSH** (Trend Micro Locality Sensitive Hashing) for fuzzy Similarity Detection
    - Comparison of Hashes, Entropy, Filenames etc.

- 📦 **Compression**
    - Huffman-Compression (implementated w/o libraries)
    - Zstandard (zstd) for professional Compression

- 🔐 **Encryption**
    - Simple XOR-based Encryption w/salt (Learning) experience
    - Safe AES encryption for real world use

- 🧰 **Recovery-Function**
    - File Recovery on .bin and .raw dumps using Magic Bytes (e.g. `.png`, `.zip`)

- 🖥 **Simple Usage**
    - Command-Line-Interface for all functions

- 🧪 **Testing**
- Catch2 unit testing framework integrated

---

## Preview and Usage

- **Commands**

<img width="962" height="308" alt="helper" src="https://github.com/user-attachments/assets/8db38112-6ca8-4998-be2a-894ff61d14d8" />

- **File Info (e.g)**

<img width="1030" height="196" alt="info" src="https://github.com/user-attachments/assets/f050fa75-aa9c-4305-b50e-bbd35e1c8f78" />


---

## 🚀 Build-Instructions

### 🔨 Needs

- C++17-compatible Compiler (z. B. `g++`, `clang++`, MSVC)
- `CMake` ≥ 3.15

### ⚙️ Compiling

```bash
git clone https://github.com/Fin-Tur/fileinsight.git
cd fileinsight
mkdir build && cd build
cmake ..
make