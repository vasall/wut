# How to install

First you need to install the necessary libraries:
- git
- gcc
- make
- build-essential
- libsdl2-dev
- libsdl2-image-dev
- freeglut3-dev

To install all libraries at once use the following command:

    apt install git gcc make build-essential libsdl2-dev libsdl2-image-dev freeglut3-dev

Now you can clone the  repository

    git clone https://github.com/vasall/wut.git

Finally to build the library we just have to change into the cloned repository and call the build script

    cd wut
    bash build.sh

If everything went successfully you should now have the static library file **lwut.a**.


# Style Attributes

| Attribute           | Value   | Description |
|---------------------|---------| -- |
| display_mode        | Keyword | Sets the visibility of the element
| width               | Flex    | Specify the width of the content space
| height              | Flex    | Specify the height of the content space
| reference_mode      | Keyword | 
| spacing_top         | Flex    |
| spacing_right       | Flex    |
| spacing_bottom      | Flex    |
| spacing_left        | Flex    |
| padding_top         | Flex    |
| padding_right       | Flex    |
| padding_bottom      | Flex    |
| padding_left        | Flex    |
| border_mode         | Keyword |
| border_width        | Flex    |
| border_color        | Color   |
| radius_top_left     | Flex    |
| radius_top_right    | Flex    |
| radius_bottom_right | Flex    |
| radius_bottom_left  | Flex    |
| infill_mode         | Keyword |
| infill_color        | Color   |
| layout_mode         | Keyword |
| align_v             | Keyword |
| align_h             | Keyword |
| scrollbar_mode      | Keyword |
| text_size           | Flex    |
| text_color          | Color   |
| text_mass           | Flex    |
| text_options        | Keyword |
| text_wrap_mode      | Keyword |
| text_spacing        | Flex    |
| line_height         | Flex    |

| Attribute       | Kexword-Values                |
|-----------------|-------------------------------|
| display_mode    | visible/hidden                |
| reference_mode  | relative/absolute             |
| border_mode     | none/solid                    |
| infill_mode     | color/none                    |
| layout_mode     | block/row/column              |
| align_v         | top/middle/bottom             |
| align_h         | left/middle/right             |
| scrollbar_mode  | auto/none/vertical/horizontal |
| text_options    | none/underline                |
| text_wrap_mode  | wordwrap/nowrap/letterwrap    |
