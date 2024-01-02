
import statistics_py
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt


class PyDataset:
    def __init__(self, csv_file):
        self._dataset = statistics_py.Dataset.from_csv(csv_file)

    def is_categorical(self, column_name):
        return self._dataset.is_categorical(column_name)

    def mean(self, column_name):
        return self._dataset.mean(column_name)

    def std_dev(self, column_name):
        return self._dataset.std_dev(column_name)

    def median(self, column_name):
        return self._dataset.median(column_name)

    def variance(self, column_name):
        return self._dataset.variance(column_name)

    def frequency_count(self, column_name):
        return self._dataset.frequency_count(column_name)

    @property
    def correlation_matrix(self):
        # Convert the Eigen matrix to a Numpy array and return it
        eigen_matrix = self._dataset.correlation_matrix
        return np.array(eigen_matrix)

    def display_correlation_matrix(self):
        # Get the correlation matrix
        corr_matrix = self.correlation_matrix

        # Plotting
        sns.heatmap(corr_matrix, annot=True, cmap='coolwarm')
        plt.title('Correlation Matrix')
        plt.show()

    def output_statistics(self, output_file):
        self._dataset.output_statistics(output_file)

    @property
    def file_name(self):
        return self._dataset.file_name

    def map_column(self, column_name, func):
        return self._dataset.map_column(column_name, func)

    def filter_rows(self, column_name, func):
        return self._dataset.filter_rows(column_name, func)

    def __iter__(self):
        # Iterate over the C++ dataset and convert each data_row to a Python list
        for i in range(len(self._dataset)):
            yield self._dataset[i]

    def __getitem__(self, i):
        # Assumes __getitem__ on C++ side returns a data row that can be directly used here
        return self._dataset[i]

    def __len__(self):
        return len(self._dataset)

    def __repr__(self):
        return f"<PyDataset named {self.file_name}>"
