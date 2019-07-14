import React, {Component} from 'react'
import styled from 'styled-components'
import WifiIcon from './wifi-icon'

export default class WifiAp extends Component {
    constructor(props) {
        super(props);
        this.state = {
            name: "",
            signalStrength: 0
        }
    }

    componentDidMount() {
        this.setState({
            name: this.props.name,
            signalStrength: this.props.signalStrength
        });
    }
    
    render() {
        return (
            <ApWrap>
                <WifiName>{this.state.name}</WifiName>
                <IconWrapper>
                    <WifiIcon signalStrength={this.state.signalStrength} width="2.5em" height="2.5em"/>
                </IconWrapper>
            </ApWrap>
        );
    }
}

/*<form style={{display: "flex", flexDirection: "row"}}>
    <input/><br/>
    <input type="submit" />
</form>*/

const ApWrap = styled.div`
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    padding: 0.5em 0em;
    margin: 1em;
    display: flex;
    background-color: ${props => props.theme.secondary};
`;

const WifiName = styled.p`
    align-self: left;
    font-size: 1.7em;
    position: relative;
    left: 0.3em;
    color: ${props => props.theme.secondaryText};
`;

const IconWrapper = styled.div`
    align-self: center;
    position: relative;
    right: 1em;
`;