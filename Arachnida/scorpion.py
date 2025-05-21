import sys
import os
import argparse
from PIL import Image, ExifTags

target_element = {'png', 'jpg', 'bmp', 'gif'}

def main():
    parser = argparse.ArgumentParser(prog= "Scorpion", description="Display MetaData")
    parser.add_argument("img", type=str, help="Path to image", nargs='+')
    args = parser.parse_args()

    for img in args.img:
        if img.split('.')[-1] in target_element:
            try:
                file = Image.open(img)
                print(f'Image : {img}')
                exif_data = file._getexif()
                if exif_data:
                    for key, val in exif_data.items():
                        if key in ExifTags.TAGS:
                            print(f'{ExifTags.TAGS[key]} : {val}')
                        else:
                            print(f'{key}:{val} (key name invalid)')

            except FileNotFoundError as e:
                print(f'[{img}] Not found')

if __name__ == "__main__":
    sys.exit(main())