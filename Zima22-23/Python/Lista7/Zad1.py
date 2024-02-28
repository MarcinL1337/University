import re
import urllib.request
import requests
from bs4 import BeautifulSoup
import threading
import queue

address = 'https?://[-a-zA-Z0-9/.]+'
automat = re.compile(address)
visited_links = []
lock = threading.Lock()


def page_text(link, text_queue):
    try:
        with urllib.request.urlopen(link) as f:
            text = f.read().decode('utf-8')
        text_queue.put(text)
    except:
        text_queue.put("")


def search_for_links(text):
    soup = BeautifulSoup(text, 'html.parser')
    links = []
    for link in soup.find_all("a"):
        if link.get('href') is not None and re.match(automat, link.get('href')):
            links.append(link.get('href'))
    return links


def crawl(start_page, distance, action, q):
    if distance == 0:
        return
    global lock

    result_queue = queue.Queue()
    thread = threading.Thread(target=page_text, args=(start_page, result_queue))
    thread.start()
    text = result_queue.get()
    result_queue.task_done()
    result_queue.join()
    thread.join()

    lock.acquire()
    global visited_links
    if start_page in visited_links:
        lock.release()
        return

    visited_links.append(start_page)
    q.put((start_page, action(text)))
    lock.release()
    links = search_for_links(text)
    threads = []

    lock.acquire()

    for link in links:
        if link not in visited_links:
            threads.append(threading.Thread(target=crawl, args=(link, distance - 1, action, q)))
    lock.release()

    for thread in threads:
        thread.start()
        
    for thread in threads:
        thread.join()


def crawl_wrapper(start_page, distance, action):
    q = queue.Queue()
    thread = threading.Thread(target=crawl, args=(start_page, distance, action, q))
    thread.start()
    thread.join()
    while not q.qsize() == 0:
        print(q.get(False))
        q.task_done()
    q.join()
    return


crawl_wrapper("https://zapisy.ii.uni.wroc.pl/", 3, lambda text: 'No' in text)

