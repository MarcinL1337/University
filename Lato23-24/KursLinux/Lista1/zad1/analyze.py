import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import os
import re

results_path = "/home/marcin/UWr/University/Lato23-24/KursLinux/Lista1/zad1/"

versions = []
person_years = []
person_months = []
total_costs = []

py_regex = re.compile(r'Development Effort Estimate, Person-Years \(Person-Months\) = ([\d,]+\.?\d*) \(([\d,]+\.?\d*)\)')
cost_regex = re.compile(r'Total Estimated Cost to Develop\s+= \$([\d,]+)')

for version in os.listdir(results_path):
    sloccount_file = os.path.join(results_path, version, "results", "sloccount_results_" + version.lstrip("linux-") + ".txt")
    if os.path.isfile(sloccount_file):
        with open(sloccount_file, 'r') as file:
            data = file.read()
            
            total_cost_match = re.search(r'Total Estimated Cost to Develop\s+= \$ ([\d,]+)', data)
            data = data.replace(",", "")
            person_years_match = re.search(r'Development Effort Estimate Person-Years \(Person-Months\) = ([\d.]+) \(([\d.]+)\)', data)


            if person_years_match and total_cost_match:
                versions.append(f'{version.split("-")[2]}-{version.split("-")[1]}')
                person_years.append(float(person_years_match.group(1)))
                person_months.append(float(person_years_match.group(2)))
                total_costs.append(int(total_cost_match.group(1).replace(',', '')))

df = pd.DataFrame({
    'Version': versions,
    'Person-Years': person_years,
    'Person-Months': person_months,
    'Total Cost ($)': total_costs
})
df.sort_values(by='Version', inplace=True)

fig, ax1 = plt.subplots(figsize=(28, 12))

color = 'tab:red'
ax1.set_xlabel('Version')
ax1.set_ylabel('Person-Years', color=color)
line1, = ax1.plot(df['Version'], df['Person-Years'], color=color, label='Person-Years', marker='o')
ax1.tick_params(axis='y', labelcolor=color)
plt.xticks(rotation=45)

# Dodajemy adnotacje do wykresu dla 'Person-Years'
for i, (py, cost) in enumerate(zip(df['Person-Years'], df['Total Cost ($)'])):
    ax1.annotate(f'{py:.2f}\n$ {cost:,.0f}', (i, py), textcoords="offset points", xytext=(0,10), ha='center', color=color)

ax2 = ax1.twinx()
color = 'tab:blue'
ax2.set_ylabel('Total Cost ($)', color=color)
line2, = ax2.plot(df['Version'], df['Total Cost ($)'], color=color, label='Total Cost ($)', marker='x')
ax2.tick_params(axis='y', labelcolor=color)

# Formatowanie osi Y dla większej dokładności
ax2.yaxis.set_major_formatter(ticker.FuncFormatter(lambda x, p: f'${x:,.0f}'))

fig.tight_layout()
plt.title('Development Effort and Total Cost over Different Versions of Linux Kernel')

# Poprawione dodawanie legendy z uwzględnieniem obu linii
plt.legend([line1, line2], ['Person-Years', 'Total Cost ($)'], loc="upper left")

plt.show()
