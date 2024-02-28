import urllib.request
import re
import requests
from urllib.request import Request, urlopen
from bs4 import BeautifulSoup

adres = 'https?://[-a-zA-Z0-9/.]+'
automat = re.compile(adres)
visited_links = []

def page_text(link):
    text = ""
    try:
        with urllib.request.urlopen(link) as f:
            text = f.read().decode('utf-8')
        return text
    except:
        return ""


def search_for_links(start_page):
    grab = requests.get(start_page)
    soup = BeautifulSoup(grab.text, 'html.parser')
    
    links = []
    for link in soup.find_all("a"):
        if link.get('href') != None and re.match(automat, link.get('href')):
            links.append(link.get('href'))
    return links

def crawl(start_page, distance, action):
    if distance > 0:
        text = page_text(start_page)
        global visited_links
        visited_links.append(start_page)
        yield (start_page, action(text))

        links = search_for_links(start_page)
        for link in links:
            if link not in visited_links:
                yield from crawl(link, distance-1, action)


for url, wynik in crawl("https://zapisy.ii.uni.wroc.pl/", 3, lambda tekst : 'Python' in tekst):
    print(f"{url}: {wynik}")

print(len(visited_links))