import React, {Component} from 'react'
import styled from 'styled-components'
import NavBar from './navbar'
import NavButton from './navbutton'


export class Header extends Component {
    render() {
        return (
            <>
                <NavBar>
                    <NavButton to="/">test1</NavButton>
                    <NavButton to="/">test2</NavButton>
                </NavBar>
                <Banner>
                    {this.props.pageName}
                </Banner>
            </>
        )

    }
}

const withHeader = (WrappedComponent, pageName) => {
    return class WithCounter extends Component
    {
        render() {
            return (
                <div>
                    <Header pageName={pageName}/>
                    <WrappedComponent/>
                </div>
            );
        }

    }
}
export default withHeader;

const Banner = styled.div`
    background-color: ${props => props.theme.primary};
    color: ${props => props.theme.primaryText};
    height: 5em;
    font-size: 1.7em;
    display: flex;
    text-align: left;
    align-items: center;
    padding: 0 0 0 1em; 
`;

const BannerText = styled.div`
    font-size: large;
    text-align: left;
    justify-content: left;
    align-items: center;
    display: flex;
    color: ${props => props.theme.primaryText};
`;