import pandas as pd
import xml.etree.ElementTree as ET
import matplotlib
matplotlib.use('TkAgg')
import seaborn as sns
import matplotlib.pyplot as plt
import nltk
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from nltk.stem import PorterStemmer

nltk.download('stopwords')
nltk.download('punkt')

tree = ET.parse('Dataset/post.xml')

# Names of the XML data fields
fields = ['id', 'PostTypeId', 'ParentId', 'AcceptedAnswerId', 'CreationDate',
          'Score', 'ViewCount', 'Body', 'OwnerUserId', 'LastEditorUserId',
          'LastEditorDisplayName', 'LastEditDate', 'LastActivityDate', 'Title',
          'Tags', 'AnswerCount', 'CommentCount', 'FavoriteCount', 'CommunityOwnedDate']

# Initialize dictionary with empty lists
data = {field: [] for field in fields}

for post in tree.getroot().findall('row'):
    for field in fields:
        data[field].append(post.get(field))

df = pd.DataFrame(data)

# Columns to remove
remove_cols = ['AcceptedAnswerId','OwnerUserId', 'LastEditorUserId',
               'LastEditorDisplayName','LastEditDate', 'LastActivityDate', 'CommunityOwnedDate']
df.drop(columns=remove_cols, inplace=True)

df['CreationDate'] = pd.to_datetime(df['CreationDate'])
df_question = df.loc[df['PostTypeId'] == '1'].copy().drop(columns=['PostTypeId', 'ParentId'])
df_question['BodyLength'] = df_question['Body'].str.len()
df_question['TitleLength'] = df_question['Title'].str.len()

columns_to_convert = ['id','Score', 'ViewCount', 'AnswerCount', 'CommentCount', 'FavoriteCount']

for col in columns_to_convert:
    df_question[col] = df_question[col].fillna(0).astype(int)

def clean_tags(tags_string):
    return tags_string.replace('><', ' ').replace('<', '').replace('>', '').replace('-', ' ')

df_question['Cleaned_Tags'] = df_question['Tags'].apply(clean_tags)

# Word count function
count_words = lambda sentence: len(sentence.split())

df_question['tags'] = df_question['Cleaned_Tags'].apply(count_words)

stop_words = set(stopwords.words('english'))
ps = PorterStemmer()

def similarity(query, document):
    result = []
    for q, d in zip(query, document):
        filtered_query = [ps.stem(w) for w in word_tokenize(q) if not w.lower() in stop_words]
        filtered_document = [ps.stem(w) for w in word_tokenize(d) if not w.lower() in stop_words]
        intersection = set(filtered_query) & set(filtered_document)
        result.append(len(intersection) / len(set(filtered_query)) * 100)
    return result

df_question['sim_Tags_Body'] = similarity(df_question['Cleaned_Tags'].str.lower(), df_question['Body'].str.lower())

column_to_ignore = ['id', 'CreationDate','Body','Title','Cleaned_Tags']

numeric_df_question = df_question.drop(columns=column_to_ignore).drop('Tags', axis=1)

correlation_matrix = numeric_df_question.corr()

fig = plt.figure()
fig.patch.set_facecolor('None')
heatmap = sns.heatmap(correlation_matrix, annot=True, cmap='coolwarm', fmt='.2f')
plt.title('Correlation Heatmap (Questions)')
heatmap.patch.set_facecolor('None')
heatmap.patch.set_alpha(0.1)
plt.figure(figsize=(12,10))  # adjust the size of the plot
sns.set(font_scale=1.2)  # scale the font for better visualization

correlation_matrix = numeric_df_question.corr()

heatmap = sns.heatmap(correlation_matrix, annot=True, cmap='coolwarm', fmt='.2f', linewidths=.05)
heatmap.set_title('Correlation Heatmap (Questions)', fontdict={'fontsize':18}, pad=16)

plt.xticks(rotation=90)
plt.yticks(rotation=0)

plt.savefig('heatmap.png', dpi=1200, bbox_inches='tight')
