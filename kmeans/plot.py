### CODE ADAPTED FROM: https://pythonprogramminglanguage.com/kmeans-clustering-centroid/
### THIS CODE IS SIMPLY FOR PLOTTING THE DATASETS INTO CLUSTERS AND USING ELBOW METHOD TO DETERMINE # OF CLUSTERS

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import sklearn
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import KMeans
from sklearn import metrics

# Read input
df = pd.read_csv(
    'dataset3.txt', sep=" ",header=None)

x = np.array(df[0])
y = np.array(df[1])

# Print statements
print(df)
print(df.describe())
print(df.columns.values)
print(x)
print(y)

# Plot scatter plots
plt.scatter(x, y, color='white', marker='o', edgecolor='red', s=50)
plt.grid()
plt.tight_layout()
plt.show()

# Plot elbow method
distortions = []
K = range(1,20)
for k in K:
    kmeanModel = KMeans(n_clusters=k)
    kmeanModel.fit(df)
    distortions.append(kmeanModel.inertia_)

plt.figure(figsize=(16,8))
plt.plot(K, distortions, 'bx-')
plt.xlabel('k')
plt.ylabel('Distortion')
plt.title('The Elbow Method showing the optimal k')
plt.show()

