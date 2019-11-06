import React, {Component} from 'react';
import withHeader from '../components/header'
import WifiApList, { apObject } from '../components/wifi-list'
import styled from 'styled-components'

class Wifi extends Component {
    constructor(props) {
        super(props);
        
        this.apList = [
            apObject("Fjell og sverd", 2),
            apObject("Fjell og sverd 5G", 80),
            apObject("Test", 60),
            apObject("test2 og sverd 5G", 20),
            apObject("Eduroam", 30),
            apObject("Blargh", 50)
        ];

        this.state = {
            availableWifi : []
        };
    }

    requestWifi = async () => {
        try {
            let response = await fetch("/aplist");
            let data = await response.json();

            console.log(JSON.stringify(data));
            console.log([apObject(data.ssid, data.signalStrength)]);
            
            this.setState({availableWifi: [apObject(data.ssid, data.signalStrength)]}, () => console.log(this.state));
        } catch(error) {
            console.error("Error: ", error);
        }
    }


    componentDidMount() {
        this.intervalId = setInterval(this.requestWifi, 5000);
    }

    componentWillUnmount() {
        clearInterval(this.intervalId);
    }

    render() {
        return (
            <ApListWrapper>
                <WifiApList apList={this.state.availableWifi} />
            </ApListWrapper>
        );
    }
}

//export default withHeader(Wifi, "Wifi");
export default withHeader(Wifi, "Wifi");

const ApListWrapper = styled.div`
    width: 40%;
    margin: 2em;
    overflow: auto;
    height: 30em;
`;