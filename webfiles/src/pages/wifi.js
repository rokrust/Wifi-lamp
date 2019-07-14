import React, {Component} from 'react';
import withHeader from '../components/header'
import WifiApList, { apObject } from '../components/wifi-list'
import styled from 'styled-components'
var ScrollArea = require('react-scrollbar')

class Wifi extends Component {
    constructor(props) {
        super(props);
        this.state = {
            availableWifi : []
        };

        this.apList = [
            apObject("Fjell og sverd", 2),
            apObject("Fjell og sverd 5G", 80),
            apObject("Test", 60),
            apObject("test2 og sverd 5G", 20),
            apObject("Eduroam", 30),
            apObject("Blargh", 50)
        ];
    }

    /*componentDidMount() {
        fetch('') //Should receive from esp. What should the URL be?
        .then(response => response.json())
        .then(data => this.setState({ availableWifi : data }));
    }*/

    render() {
        return (
            <ApListWrapper>
                <WifiApList apList={this.apList} />
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