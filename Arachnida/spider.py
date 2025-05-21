import sys
import os
import argparse
import requests
from bs4 import BeautifulSoup

visited_urls = []
downloaded_imgs = []

header = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36"
}

target_element = {'png', 'jpg', 'bmp', 'gif'}

def downloadImg(url, path):
	try:
		if url.split('/')[-1].split('.')[-1] not in target_element:
			return
		for i in range(len(downloaded_imgs)):
			if downloaded_imgs[i] == url:
				return
		downloaded_imgs.append(url)
		name = path + '/' + url.split('/')[-1]
		result = requests.get(url, stream=True) # Setting stream=True means that requests will not download the whole response immediately.
		content_type = result.headers.get("Content-Type", "")
		if result.status_code == 200 and "image" in content_type:
			with open(name, 'wb') as f:
				for chunk in result.iter_content(1024): # Download the file in 1Kb chunk
					f.write(chunk)
			print(f"✔️ {name} downloaded")
	except requests.exceptions.RequestException as e:
		return
	

def web_crawler(url, path, depth=0):
	for i in range(len(visited_urls)):
		if visited_urls[i] == url:
			return
	visited_urls.append(url)
	print(f'Crawling in : {visited_urls[-1]}')

	result = requests.get(url, headers=header).text
	html = BeautifulSoup(result, "html.parser")

	for img in html.find_all('img'):
		src = img.get('src')
		if src and src.startswith("http"):
			downloadImg(src, path)

	if (depth > 0):
		for link in html.find_all('a'):
			href = link.get('href')
			if href and href.startswith("http"):
				web_crawler(href, path, depth - 1)


def main():
	try:
		parser = argparse.ArgumentParser(prog= "Spider", description="Scraping tool")
		parser.add_argument('url', type=str, help="URL")
		parser.add_argument('-r', action="store_true", help="Recursively or not recursively, that is the question")
		parser.add_argument('-l', type=int, default=5, help="Depth of recursive")
		parser.add_argument('-p', type=str, default="data", help="Output folder")

		# args.r .l .p .url
		args = parser.parse_args()

		if not os.path.isdir(args.p):
			os.makedirs(args.p)

		if args.r == False:
			web_crawler(args.url, args.p)
		else:
			web_crawler(args.url, args.p, args.l)
		print("✅ Finish !")
	except KeyboardInterrupt:
		print("^C used")
		return 0

if __name__ == "__main__":
	sys.exit(main())