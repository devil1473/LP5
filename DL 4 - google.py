import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import mean_squared_error
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import SimpleRNN, Dense, Dropout
import math

-------------------

df = pd.read_csv("GOOGL.csv")  # Ensure the CSV has a 'Close' column
df = df[['Date', 'Close']]
df['Date'] = pd.to_datetime(df['Date'])
df.set_index('Date', inplace=True)
df.sort_index(inplace=True)

-------------------

# 📊 Preprocess Data
data = df['Close'].values.reshape(-1, 1)
scaler = MinMaxScaler(feature_range=(0, 1))
scaled_data = scaler.fit_transform(data)

training_data_len = int(len(scaled_data) * 0.8)
train_data = scaled_data[:training_data_len]

---------------------

x_train, y_train = [], []
for i in range(60, len(train_data)):
    x_train.append(train_data[i-60:i, 0])
    y_train.append(train_data[i, 0])
x_train, y_train = np.array(x_train), np.array(y_train)
x_train = x_train.reshape(x_train.shape[0], x_train.shape[1], 1)


-----------------------

# 🧠 Build RNN Model
model = Sequential()
model.add(SimpleRNN(units=50, return_sequences=False, input_shape=(x_train.shape[1], 1)))
model.add(Dropout(0.2))
model.add(Dense(1))
model.compile(optimizer='adam', loss='mean_squared_error')
model.summary()

------------------------

# 🔁 Train the Model
model.fit(x_train, y_train, epochs=20, batch_size=32)

-----------------------

# 📈 Prepare Test Data
test_data = scaled_data[training_data_len - 60:]
x_test = []
y_test = data[training_data_len:]
for i in range(60, len(test_data)):
    x_test.append(test_data[i-60:i, 0])
x_test = np.array(x_test)
x_test = x_test.reshape(x_test.shape[0], x_test.shape[1], 1)

-------------------------

# 🔮 Predictions
predictions = model.predict(x_test)
predictions = scaler.inverse_transform(predictions)

----------------------

# 📉 Evaluate the Model
rmse = math.sqrt(mean_squared_error(y_test, predictions))
print("Root Mean Squared Error:", rmse)

-----------------------

# 📊 Plot Results
train = df[:training_data_len]
valid = df[training_data_len:]
valid['Predictions'] = predictions

----------------------

plt.figure(figsize=(14, 7))
plt.title('Google Stock Price Prediction using RNN')
plt.xlabel('Date')
plt.ylabel('Close Price USD')
plt.plot(train['Close'], label='Training Data')
plt.plot(valid[['Close', 'Predictions']], label=['Actual', 'Predicted'])
plt.legend()
plt.show()