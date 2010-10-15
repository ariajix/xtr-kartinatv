<?php


class PlaylistItem {
    var $number;
    var $length;
    var $name;
    var $uri;
    
    function PlaylistItem ($number, $uri, $descr = null) {
    	$this->number	= $number;
        $this->uri		= $uri;
        if(null != $descr) {
			$properties = explode(',',$descr);
			$this->length	= $properties[0] ;
			$this->name	= $properties[1] ;
        }
        
    }
}

class M3uParser {
    
    var $itemsAmount;
    var $items;
    var $name;
    var $uri;

    function M3uParser() {
        $this->itemsAmount = 0;
        $this->items = array();
    }
    
    function parseFile($fileUri,$name) {
    	$this->name = $name;
    	$this->uri  = $fileUri;
    	$lines = file($fileUri);
		
		foreach ($lines as $line_num => $line) {
			//trim UTF8 BOM in first line if any
			if(0 == $line_num) {
				$fi = ord(substr($line,0,1));
				$se = ord(substr($line,1,1));
				$th = ord(substr($line,2,1));				
				if(239 == $fi && 187 == $se && 191 == $th) {
					$line = substr($line,3,7);
				}
			}
			
			$line = trim($line);
		    if(0 == strncasecmp($line, "#EXTM3U",7)) {
		    	//skip
		    }
		    else if(0 == strncasecmp($line, "#EXTINF:",8)){
		    	$descr = substr($line,8); 
		    }
		    else if(0 != strncasecmp($line, "#",1)){
		    	$item = new PlaylistItem($this->itemsAmount, $line, $descr);
                $descr = null;
                $this->items[] = $item;
                $this->itemsAmount++;
		    	
		    }
		}    	
    }

    function parse($string) {
		# compact all to a single line
		$content = str_replace("\n","",$string);
		
		# no spaces between tags
		$content = str_replace('>[[:space:]]+', '>', $content);
		$content = str_replace('[[:space:]]+<', '<', $content);
		
		# parse out channels
		$channels = explode('><',$content);    	

		$i=0;
		foreach($channels as $k => $v) {
		  $i=$i+1;
			if(strstr ($v,"channelgroup ")) {

				ereg('id="([^"]*)"', $v, $matches_id);
				ereg('title="([^"]*)"', $v, $matches_name);
				ereg('color="([^"]*)"', $v, $matches_color);

				$category = new Category(
                        $matches_id[1], $matches_name[1], $matches_color[1]);
                
                $this->categories[] = $category;
			}
			if(strstr ($v,"channel ")) {
				$matches_id = null;
				$matches_name = null;
				$matches_idx = null;
				$matches_programm = null;
				$matches_sprog = null;
				$matches_eprog =  null;
								
				ereg('id="([^"]*)"', $v, $matches_id);
				ereg('title="([^"]*)"', $v, $matches_name);
				ereg('idx="([^"]*)"', $v, $matches_idx);
				ereg('programm="([^"]*)"', $v, $matches_programm);
				ereg('sprog="([^"]*)"', $v, $matches_sprog);
				ereg('eprog="([^"]*)"', $v, $matches_eprog);
				 $channel = new Channel(
                        $matches_idx[1], $matches_id[1], $matches_name[1], 
                        $matches_programm[1], $matches_sprog[1], $matches_eprog[1]);
                    $this->channelsAmount++;

/*                    if (array_key_exists('SELECTED', $attr)) {
                        $this->selectedChannel = $channel->number;
                    }
*/
                    if (count($this->categories) == 0) {
                        $this->categories[] = 
                            new Category(0, "No name", "black");
                    }

                    $last = count($this->categories) - 1;
                    $this->categories[$last]->addChannel($channel);
			}
		}		
		
        if (CL_CUSTOM_SORTING) {            
            $this->categories = Sorter::sortCategories($this->categories);
            $this->channelsAmount = Sorter::getChannelsAmount($this->categories);
        }
        
    }        
}


?>