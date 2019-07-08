import React, {Component} from 'react'

export default function WifiIcon(props)
{
    const black = "black"
    const grey = "grey"

    return (
        <svg version="1.1" id="Capa_1" xmlns="http://www.w3.org/2000/svg" xmlnsXlink="http://www.w3.org/1999/xlink" x="0px" y="0px"
	        viewBox="0 0 489.3 489.3" xmlSpace="preserve" width={props.height} height={props.height}>
            <g>
	            <g>
                    <path fill={props.signalStrength > 75 ? black : grey} d="M481.65,157.675c-130.7-130.6-343.3-130.6-474,0c-10.2,10.2-10.2,26.8,0,37.1c10.2,10.2,26.8,10.2,37.1,0
			            c110.2-110.3,289.6-110.3,399.9,0c5.1,5.1,11.8,7.7,18.5,7.7s13.4-2.6,18.5-7.7C491.85,184.575,491.85,167.975,481.65,157.675z"/>
		            <path fill={props.signalStrength > 50 ? black : grey} d="M79.55,229.675c-10.2,10.2-10.2,26.8,0,37.1c10.2,10.2,26.8,10.2,37.1,0c70.6-70.6,185.5-70.6,256.1,0
			            c5.1,5.1,11.8,7.7,18.5,7.7s13.4-2.6,18.5-7.7c10.2-10.2,10.2-26.8,0-37.1C318.75,138.575,170.55,138.575,79.55,229.675z"/>
		            <path fill={props.signalStrength > 25 ? black : grey} d="M150.35,300.475c-10.2,10.2-10.2,26.8,0,37.1c10.2,10.2,26.8,10.2,37.1,0c31.5-31.6,82.9-31.6,114.4,0
			            c5.1,5.1,11.8,7.7,18.5,7.7s13.4-2.6,18.5-7.7c10.2-10.2,10.2-26.8,0-37C286.95,248.475,202.35,248.475,150.35,300.475z"/>
		            <circle cx="244.65" cy="394.675" r="34.9"/>
	            </g>
            </g>
        </svg>
    )
}