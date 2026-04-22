import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv('search_times.csv')
sizes = df['size']
linear = df['linear']
bst = df['bst']
rbt = df['rbt']
hash_time = df['hash']
mmap = df['multimap']
collisions = df['collisions']

plt.figure(figsize=(12, 5))

# График времени поиска
plt.subplot(1, 2, 1)
plt.plot(sizes, linear, 'o-', label='Linear')
plt.plot(sizes, bst, 's-', label='BST')
plt.plot(sizes, rbt, '^-', label='RBTree')
plt.plot(sizes, hash_time, 'd-', label='HashTable')
plt.plot(sizes, mmap, 'v-', label='std::multimap')
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Array size (n)')
plt.ylabel('Time (seconds)')
plt.title('Search Time Comparison')
plt.grid(True, which='both', linestyle='--', alpha=0.7)
plt.legend()

# График коллизий хэш-таблицы
plt.subplot(1, 2, 2)
plt.plot(sizes, collisions, 'o-', color='red')
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Array size (n)')
plt.ylabel('Collisions count')
plt.title('Hash Table Collisions')
plt.grid(True, which='both', linestyle='--', alpha=0.7)

plt.tight_layout()
plt.savefig('search_plots.png', dpi=150)
plt.show()