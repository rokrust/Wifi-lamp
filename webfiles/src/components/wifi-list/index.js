import React, {Component} from 'react'
import styled from 'styled-components'
import WifiAp from './wifi-ap'

export default class WifiApList extends Component {
    constructor(props) {
        super(props);
        this.sortedAp = this.props.apList.sort((a, b) => b.signalStrength - a.signalStrength)
    }
    

    render() {
        return (
            <>
                { this.sortedAp.map(ap => <WifiAp name={ap.name} signalStrength={ap.signalStrength}/>) }
            </>
        )
    }
}

export let apObject = (name, signalStrength) => { return { name, signalStrength } }

const WifiList = styled.div`
`;